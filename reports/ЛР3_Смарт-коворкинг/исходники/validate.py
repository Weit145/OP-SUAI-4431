"""Проверка согласованности артефактов ЛР3; запускать после export.py."""
from pathlib import Path
from zipfile import ZipFile
import xml.etree.ElementTree as X
import re,json,subprocess,shutil,sys,os
from model import ENTITIES,RELATIONS,STORES,SCENARIOS
B=Path(__file__).resolve().parent;P=B.parent/'ЛР3_Смарт-коворкинг_Кочнев_4431.docx';N={'w':'http://schemas.openxmlformats.org/wordprocessingml/2006/main'}
P=Path(os.environ.get('LR3_REPORT_PATH',str(P)))
entity_names={row[0] for row in ENTITIES};covered=[name for store in STORES for name in store[2]]
assert len(covered)==len(set(covered))==21 and set(covered)==entity_names
for parent,child,fk,kind,card,pc in RELATIONS:
 assert parent in entity_names and child in entity_names
 e=next(e for e in ENTITIES if e[0]==child)
 assert fk in {a.split()[0].rstrip('?') for a in e[2]+e[3]},(child,fk)
 if kind=='ID':assert fk in {a.split()[0].rstrip('?') for a in e[2]}
with ZipFile(P) as z:
 assert z.testzip() is None
 root=X.fromstring(z.read('word/document.xml'))
 for name in z.namelist():
  if name.endswith('.xml'):X.fromstring(z.read(name))
rows=[]
for row in root.findall('.//w:tr',N):
 cells=row.findall('w:tc',N)
 if not cells:continue
 ident=''.join(t.text or '' for t in cells[0].findall('.//w:t',N))
 if re.fullmatch(r'ФТ-\d{2}',ident):rows.append(ident)
assert len(rows)==len(set(rows))==94 and set(rows)=={f'ФТ-{n:02}' for n in range(1,95)}
manifest=json.loads((B/'manifest.json').read_text());count=len(manifest)
assert len(root.findall('.//w:drawing',N))==count
if shutil.which('pdftotext'):
 ps=subprocess.run(['pdftotext','-layout',str(P.with_suffix('.pdf')),'-'],capture_output=True,text=True,check=True).stdout
else:
 sys.path.insert(0,str(B.parent/'инструменты'/'python'))
 from pypdf import PdfReader
 ps=''.join((page.extract_text() or '')+'\f' for page in PdfReader(P.with_suffix('.pdf')).pages)
pages=ps.split('\f')[:-1]
assert pages and not any(len(p.strip())<12 for p in pages)
figures=re.findall(r'Рисунок\s+(\d+)\s+—',ps)
assert sorted(map(int,figures))==list(range(1,count+1))
assert not re.search('Ошибка!|Error!|Источник ссылки не найден',ps)
assert '20 минут' in re.sub(r'\s+',' ',ps)
for name in entity_names:assert name in ps,name
mx=X.parse(B/'ЛР3_Смарт-коворкинг.drawio').getroot()
assert len(mx.findall('diagram'))==count
for page in mx.findall('diagram'):
 cells=page.findall('.//mxCell');ids=[c.get('id') for c in cells];assert len(ids)==len(set(ids))
 for c in cells:
  for link in ['source','target']:
   if c.get(link):assert c.get(link) in ids
# Compare each DFD sheet with expanded directed flows and store ownership.
from revised import dfd_edges
from collections import Counter
semantic_edges=dfd_edges()
for page in mx.findall('diagram'):
 if not re.fullmatch(r'0[456][bcd]?_DFD_P[1-6]',page.get('name','')):continue
 process=page.get('name').split('_')[-1]
 actual=Counter((c.get('sourceCode'),c.get('targetCode'),c.get('flowCode')) for c in page.findall('.//mxCell') if c.get('sourceCode'))
 expected=Counter((a,b,code) for a,b,code,_ in semantic_edges if process in (a,b))
 assert actual==expected,(process,actual-expected,expected-actual)
 assert all(not(a.startswith('D') and b.startswith('D')) for a,b,_ in actual)
 assert all(not(a.startswith('E') and b.startswith('D') or a.startswith('D') and b.startswith('E')) for a,b,_ in actual)
assert {c for a,b,c,t in semantic_edges if c}=={f'F{i:02}' for i in range(1,31)}
# Every asset referenced by the manifest is present, and SVG remains valid XML.
for entry in manifest:
 assert (B/'диаграммы'/entry['png']).is_file()
 X.parse(B/'диаграммы'/(entry['key']+'.svg'))
result={'pages':len(pages),'functional_requirements':94,'diagrams':count,'stores':8,'entities':21,'relationships':len(RELATIONS),'scenarios':len(SCENARIOS),'dfd_directed_edges':len(semantic_edges),'checks':'passed'}
(B.parent/'Результат_проверки.json').write_text(json.dumps(result,ensure_ascii=False,indent=2))
print(json.dumps(result,ensure_ascii=False))
