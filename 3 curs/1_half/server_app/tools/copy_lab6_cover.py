"""Copy the actual Word cover from Lab 6, including its table layout and styles."""
from copy import deepcopy
from pathlib import Path
import re
from zipfile import ZipFile, ZIP_DEFLATED
from xml.etree import ElementTree as ET

root = Path(__file__).resolve().parents[1]
source = root / "ОтчетЛР6Кочнев4431.docx"
target = root / "ОтчетЛР7Кочнев4431.docx"
W = "{http://schemas.openxmlformats.org/wordprocessingml/2006/main}"
prefix = "Cover6_"


def load(path):
    with ZipFile(path) as archive:
        return {name: archive.read(name) for name in archive.namelist()}


def namespaces(data):
    return dict(re.findall(r'xmlns:([\w]+)="([^"]+)"', data.decode()))


def serialize(tree, original):
    # Preserve declarations used by mc:Ignorable and other namespace-valued attributes.
    data = ET.tostring(tree, encoding="unicode")
    end = data.index(">")
    head = data[:end]
    for name, uri in namespaces(original).items():
        if f"xmlns:{name}=" not in head:
            head += f' xmlns:{name}="{uri}"'
    return ('<?xml version="1.0" encoding="UTF-8" standalone="yes"?>\n' + head + data[end:]).encode()


src, dst = load(source), load(target)
for name, uri in {**namespaces(src["word/document.xml"]), **namespaces(dst["word/document.xml"])}.items():
    ET.register_namespace(name, uri)
src_doc = ET.fromstring(src["word/document.xml"])
dst_doc = ET.fromstring(dst["word/document.xml"])
src_body, dst_body = src_doc.find(W + "body"), dst_doc.find(W + "body")


def start(body):
    return next(i for i, child in enumerate(body)
                if "".join(t.text or "" for t in child.iter(W + "t")).strip() == "Задание:")


cover = [deepcopy(element) for element in list(src_body)[:start(src_body)]]
style_refs = {W + tag for tag in ("pStyle", "rStyle", "tblStyle", "basedOn", "next", "link")}
for element in cover:
    for paragraph in element.iter(W + "p"):
        texts = list(paragraph.iter(W + "t"))
        if "ОТЧЕТ О ЛАБОРАТОРНОЙ РАБОТЕ" in "".join(t.text or "" for t in texts):
            for text in texts:
                text.text = (text.text or "").replace("№6", "№7")
                if text.text == "6":
                    text.text = "7"
        for text in texts:
            if text.text == "РАЗРАБОТКА ПРИЛОЖЕНИЯ С ИСПОЛЬЗОВАНИЕМ HIBERNATE":
                text.text = "РАЗРАБОТКА ПРИЛОЖЕНИЯ С АСИНХРОННОЙ ОЧЕРЕДЬЮ СООБЩЕНИЙ"
    for node in element.iter():
        if node.tag in style_refs and W + "val" in node.attrib:
            node.set(W + "val", prefix + node.get(W + "val"))

for element in list(dst_body)[:start(dst_body)]:
    dst_body.remove(element)
for index, element in enumerate(cover):
    dst_body.insert(index, element)

src_styles = ET.fromstring(src["word/styles.xml"])
dst_styles = ET.fromstring(dst["word/styles.xml"])
for style in list(dst_styles):
    if style.get(W + "styleId", "").startswith(prefix):
        dst_styles.remove(style)
for style in src_styles.findall(W + "style"):
    style = deepcopy(style)
    style.set(W + "styleId", prefix + style.get(W + "styleId"))
    style.attrib.pop(W + "default", None)
    for node in style.iter():
        if node.tag in style_refs and W + "val" in node.attrib:
            node.set(W + "val", prefix + node.get(W + "val"))
        if node.tag == W + "name":
            node.set(W + "val", prefix + node.get(W + "val"))
    dst_styles.append(style)

dst["word/document.xml"] = serialize(dst_doc, dst["word/document.xml"])
dst["word/styles.xml"] = serialize(dst_styles, dst["word/styles.xml"])
temporary = target.with_suffix(".cover.tmp.docx")
with ZipFile(temporary, "w", ZIP_DEFLATED) as archive:
    for name, content in dst.items():
        archive.writestr(name, content)
temporary.replace(target)
print("Copied the Lab 6 Word cover into Lab 7")
