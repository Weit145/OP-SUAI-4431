from functools import partial
from http.server import HTTPServer, SimpleHTTPRequestHandler
from pathlib import Path


ROOT = Path(__file__).resolve().parent


class Handler(SimpleHTTPRequestHandler):
    def do_GET(self):
        if self.path == "/":
            self.path = "/index.html"
        super().do_GET()


if __name__ == "__main__":
    port = 8000
    handler = partial(Handler, directory=ROOT)
    print(f"Страница открывается по адресу: http://127.0.0.1:{port}")
    HTTPServer(("127.0.0.1", port), handler).serve_forever()
