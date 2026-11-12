"""Build Process Modeler XML using its own exported schema and object prototypes."""
from pathlib import Path
import xml.etree.ElementTree as E
import copy,uuid
from model import ICOM
BASE=Path(__file__).resolve().parent
TOOLS=BASE.parent/'инструменты'
NS='http://www.ca.com/ProcessModeler'
E.register_namespace('',NS)
def tag(s):return '{'+NS+'}'+s
def find(e,s):return e.find('.//'+tag(s))
def put(e,name,value,member=None):
 p=e.find(tag(name)+(f"[@member_name='{member}']" if member else ''))
 if p is None:p=E.SubElement(e,tag(name),{'member_name':member} if member else {})
 p.text=str(value)
def members(e,name,names,values):
 for n,v in zip(names,values):put(e,name,v,n)
def uid():return '{'+str(uuid.uuid4()).upper()+'}+00000000'
sample=E.parse(TOOLS/'sample-template.xml').getroot()
root=E.parse(TOOLS/'native-template.xml').getroot()
model=find(root,'PMModel');mp=model.find(tag('PMModelProps'))
put(mp,'Name','Смарт-коворкинг');model.set('Name','Смарт-коворкинг')
put(mp,'PMAuthor','Д. О. Кочнев');put(mp,'PMPurpose','Структурное проектирование ИС по требованиям ЛР2')
put(mp,'PMDisplayShadowsFlag',0);put(mp,'PMDisplayLeafCornersFlag',0)
put(mp,'PMDisplayBoxHighlightingFlag',0);put(mp,'PMFitActivityTextInBoxFlag',1)
put(mp,'PMShowActivityCostFlag',0)
for p in root.iter():
 if p.tag in [tag('PMShowIDEF0KitFlag'),tag('PMShowIDEF0TitleFlag')]:p.text='0'
 if p.tag==tag('PMLogfont') and p.get('member_name')=='lfCharSet':p.text='204'
 if p.tag==tag('PMLogfont') and p.get('member_name')=='lfFaceName':p.text='Arial'
 if p.tag==tag('PMZoomScale'):p.text='90'
 for name in ['PMDisplayColorsFlag']: 
  if p.tag==tag(name):p.text='0'
d=find(root,'PMDiagram');dp=d.find(tag('PMDiagramProps'))
a=find(root,'PMActivity');ap=a.find(tag('PMActivityProps'))
title='Обеспечить бронирование\nи обслуживание клиентов\nсмарт-коворкинга'
put(ap,'Name',title);a.set('Name',title);put(dp,'Name',title);d.set('Name',title)
box=find(d,'PMBox');bp=box.find(tag('PMBoxProps'))
members(bp,'PMBoxCoordinates',['left','top','right','bottom'],[400,210,720,390])
font=find(dp,'PMDefaultArrowFontRef').text
arrows=E.SubElement(model,tag('PMArrow_Groups'))
labels=E.SubElement(d,tag('PMArrowLabel_Groups'))
def arrow(text,source,sink,points,rect):
 ar=copy.deepcopy(find(sample,'PMArrow'));ar.set('id',uid());ar.set('Name',text)
 props=ar.find(tag('PMArrowProps'));put(props,'Name',text);put(props,'PMAuthor','Д. О. Кочнев')
 for name in ['PMNameColorReference','PMTextColorReference']:put(props,name,'')
 arrows.append(ar)
 lab=copy.deepcopy(find(sample,'PMArrowLabel'));lab.set('id',uid());lp=lab.find(tag('PMArrowLabelProps'))
 put(lp,'PMArrowRef',ar.get('id'));put(lp,'PMTextFontRef',font)
 members(lp,'PMTextBoundingBox',['left','top','right','bottom'],rect)
 members(lp,'PMSegmentConnectPoint',['x','y'],points[0]);members(lp,'PMLabelConnectPoint',['x','y'],points[0])
 seg=find(lab,'PMArrowSegment');seg.set('id',uid());sp=seg.find(tag('PMArrowSegmentProps'))
 for child in list(sp):
  if child.tag==tag('PMVertices'):sp.remove(child)
 direction=2 if points[0][0]==points[1][0] else 1
 for i,(x,y) in enumerate(points+[(0,direction)]):E.SubElement(sp,tag('PMVertices'),index=str(i)).text=f'({x},{y})'
 put(sp,'PMSourceConnectionRef',source);put(sp,'PMSinkConnectionRef',sink)
 labels.append(lab)
border={side:find(dp,'PM'+side+'Ref').text for side in ['Left','Right','Top','Bottom']}
short={r[0]:r[2] for r in ICOM}
for i in range(3):
 y=245+i*55;code='I'+str(i+1)
 arrow(code+' '+short[code],border['Left'],box.get('id'),[(0,y),(400,y)],[20,y-36,380,y-5])
for i in range(4):
 y=230+i*45;code='O'+str(i+1)
 arrow(code+' '+short[code],box.get('id'),border['Right'],[(720,y),(1109,y)],[745,y-35,1090,y-4])
for i in range(4):
 x=430+i*85;code='C'+str(i+1);y=30+(i%2)*65
 arrow(code+' '+short[code],border['Top'],box.get('id'),[(x,0),(x,210)],[x-70,y,x+90,y+55])
for i in range(3):
 x=440+i*110;code='M'+str(i+1);y=440+(i%2)*75
 arrow(code+' '+short[code],border['Bottom'],box.get('id'),[(x,615),(x,390)],[x-95,y,x+105,y+55])
OUT=BASE/'ProcessModeler';OUT.mkdir(exist_ok=True)
E.indent(root);E.ElementTree(root).write(OUT/'SmartCoworking_context.xml',encoding='utf-8',xml_declaration=True)
print('Native XML: 1 activity, 14 arrows; no frame, monochrome.')
