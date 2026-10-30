#!/usr/bin/env python3
"""Exercise two running HTTPS replicas, including real form login and CSRF."""
import argparse
import http.cookiejar
import json
import ssl
import time
import urllib.error
import urllib.parse
import urllib.request
from datetime import datetime, timezone
from html.parser import HTMLParser
from pathlib import Path


class Meta(HTMLParser):
    def __init__(self, html):
        super().__init__()
        self.values = {}
        self.feed(html)

    def handle_starttag(self, tag, attrs):
        attrs = dict(attrs)
        if tag == "meta" and attrs.get("name"):
            self.values[attrs["name"]] = attrs.get("content")


class Client:
    def __init__(self, base):
        self.base = base
        # The project intentionally uses a development self-signed certificate.
        self.opener = urllib.request.build_opener(
            urllib.request.HTTPSHandler(context=ssl._create_unverified_context()),
            urllib.request.HTTPCookieProcessor(http.cookiejar.CookieJar()))
        meta = Meta(self.request("GET", "/")[1]).values
        body = urllib.parse.urlencode({"login": "guest", "pass": "hello", "_csrf": meta["_csrf"]}).encode()
        self.request("POST", "/login", body, {"Content-Type": "application/x-www-form-urlencoded"})
        meta = Meta(self.request("GET", "/")[1]).values
        self.csrf = {meta["_csrf_header"]: meta["_csrf"]}
        assert self.request("GET", "/api/audit")[0] == 200, "Login failed"

    def request(self, method, path, body=None, headers=None):
        request = urllib.request.Request(self.base + path, data=body, method=method, headers=headers or {})
        try:
            with self.opener.open(request, timeout=15) as response:
                return response.status, response.read().decode()
        except urllib.error.HTTPError as error:
            return error.code, error.read().decode()

    def api(self, method, path, body=None):
        headers = {"Content-Type": "application/json", **self.csrf}
        status, text = self.request(method, path, json.dumps(body).encode() if body is not None else None, headers)
        return status, json.loads(text) if text else None


def wait_for(check):
    deadline = time.monotonic() + 30
    while time.monotonic() < deadline:
        if check():
            return
        time.sleep(0.2)
    raise AssertionError("Replication did not finish within 30 seconds")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--first", default="https://localhost:8443")
    parser.add_argument("--second", default="https://localhost:9443")
    parser.add_argument("--output", default="lab7-verification.json")
    args = parser.parse_args()
    first, second = Client(args.first), Client(args.second)
    body = {"address": "Проверка репликации ЛР7, " + datetime.now().isoformat(timespec="seconds"),
            "type": "APARTMENT", "area": 45.5, "rooms": 2, "monthlyRent": 50000, "available": True}
    status, created = first.api("POST", "/api/properties", body)
    assert status == 201, created
    path = "/api/properties/" + str(created["id"])
    try:
        wait_for(lambda: second.api("GET", path)[0] == 200)
        on_second = second.api("GET", path)[1]
        assert on_second == created
        body["monthlyRent"] = 65000
        assert second.api("PUT", path, body)[0] == 200
        wait_for(lambda: first.api("GET", path)[1].get("monthlyRent") == 65000)
        updated = first.api("GET", path)[1]
        assert first.api("DELETE", path)[0] == 204
        wait_for(lambda: second.api("GET", path)[0] == 404)

        def audit(client):
            return [event for event in client.api("GET", "/api/audit")[1] if event["propertyId"] == created["id"]]

        wait_for(lambda: len(audit(first)) == len(audit(second)) == 3)
        result = {"checkedAt": datetime.now(timezone.utc).isoformat(), "first": args.first,
                  "second": args.second, "createdOnFirst": created, "readOnSecond": on_second,
                  "updatedOnSecondReadOnFirst": updated, "deletedStatusOnSecond": 404,
                  "auditFirst": audit(first), "auditSecond": audit(second), "result": "PASS"}
        Path(args.output).write_text(json.dumps(result, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
        print(json.dumps(result, ensure_ascii=False, indent=2))
    finally:
        # Remove only the demonstration object created above.
        first.api("DELETE", path)


if __name__ == "__main__":
    main()
