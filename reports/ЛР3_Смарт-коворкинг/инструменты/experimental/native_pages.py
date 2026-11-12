"""Convert editable diagram geometry to native Process Modeler XML objects."""
import native_xml as g
import xml.etree.ElementTree as E
import copy,re,json
from pathlib import Path
T=g.tag
book=E.parse(next(g.BASE.glob('*.drawio'))).getroot()
prototype=copy.deepcopy(g.root)
def props(obj):return obj.find(T(obj.tag.split('}')[-1]+'Props'))
def group(parent,name):
 e=parent.find(T(name+'_Groups'))
 return e if e is not None else E.SubElement(parent,T(name+'_Groups'))
def obj(parent,name,proto=None):
 if proto is None:proto=g.find(g.sample,name)
 e=copy.deepcopy(proto) if proto is not None else E.Element(T(name))
 for child in list(e):
  if child.tag.endswith('_Groups'):e.remove(child)
 e.set('id',g.uid());e.set('Name','')
 if props(e) is None:E.SubElement(e,T(name+'Props'))
 group(parent,name).append(e)
 return e
def coord(x,y):return round(float(x)*.5),round((float(y)-100)*.5)
def setrect(p,name,x,y,w,h):
 xx,yy=coord(x,y);g.members(p,name,['left','top','right','bottom'],[xx,yy,xx+round(float(w)*.5),yy+round(float(h)*.5)])
manifest=[]
for family,method in [('IDEF0',0),('DFD',1),('IDEF3',2)]:
 pages=[p for p in book.findall('diagram') if family in p.get('name','') and 'A-0' not in p.get('name','')]
 root=copy.deepcopy(prototype);model=g.find(root,'PMModel');mp=props(model)
 name='Смарт-коворкинг — '+family;model.set('Name',name);g.put(mp,'Name',name)
 for c in list(model):
  if c.tag in [T('PMArrow_Groups')]:model.remove(c)
 rootdiagram=g.find(root,'PMDiagram');rootactivity=g.find(root,'PMActivity');rootbox=g.find(root,'PMBox')
 for child in list(rootdiagram):
  if child.tag in [T('PMArrowLabel_Groups'),T('PMNode_Groups')]:rootdiagram.remove(child)
 g.put(props(rootactivity),'PMBoxType',method+1)
 g.put(props(rootdiagram),'PMMethodologyCode',method)
 g.put(props(rootdiagram),'PMCurrentDiagram',0)
 # The original context supplies a valid hierarchy root; imported sheets are FEO views.
 g.put(mp,'PMFitActivityTextInBoxFlag',1)
 activities={};diagrams=[]
 for pageidx,page in enumerate(pages):
  key=page.get('name');cells=page.findall('.//mxCell');d=obj(model,'PMDiagram',g.find(prototype,'PMDiagram'));dp=props(d)
  d.set('Name',key);g.put(dp,'Name',key);g.put(dp,'PMMethodologyCode',method)
  g.put(dp,'PMDiagramNumber',pageidx+1);g.put(dp,'PMFEOFlag',1)
  g.put(dp,'PMDecompositionOfRef',rootbox.get('id'));g.put(dp,'PMRepresentedActivityRef',rootactivity.get('id'))
  g.put(dp,'PMParentDiagramRef',rootdiagram.get('id'));g.put(dp,'PMCurrentDiagram',16 if pageidx==0 else 0)
  g.members(dp,'PMDimension',['x','y'],[1200,750]);g.members(dp,'PMActiveAreaDimension',['x','y'],[1200,625]);g.members(dp,'PMOriginOffset',['x','y'],[0,0])
  g.put(dp,'PMZoomScale',90)
  for side in ['Left','Top','Right','Bottom']:
   b=obj(d,'PMBorder',g.find(prototype,'PMBorder'));g.put(dp,'PM'+side+'Ref',b.get('id'))
  boxes={};rects={};allpoints={}
  for cell in cells:
   if cell.get('vertex')!='1' or cell.get('id','').startswith('text'):continue
   geo=cell.find('mxGeometry')
   if geo is None:continue
   ident=cell.get('id');text=cell.get('value','');style=cell.get('style','')
   cls='PMDatastore' if ident.startswith('D') else 'PMExternal' if ident.startswith('E') else 'PMJunction' if ident.startswith('J') else 'PMActivity'
   typ=4 if cls=='PMDatastore' else 5 if cls=='PMExternal' else 11 if cls=='PMJunction' else method+1
   cache=(cls,ident) if family=='DFD' else (cls,key,ident)
   if cache not in activities:
    a=obj(model,cls,g.find(prototype,'PMActivity') if cls in ['PMDatastore','PMExternal'] else None)
    # Data stores and external references share activity properties.
    if a.tag!=T(cls):
     oldprops=props(a);a.tag=T(cls);oldprops.tag=T(cls+'Props')
    ap=props(a)
    if family=='IDEF3' and '\n' in text:text=text.split('\n',1)[1]
    g.put(ap,'Name',text);a.set('Name',text);g.put(ap,'PMBoxType',typ)
    if cls=='PMActivity':g.put(ap,'PMParentActivityRef',rootactivity.get('id'))
    else:
     for prop in list(ap):
      if prop.tag in [T(k) for k in ['PMParentActivityRef','PMFrequency','PMDuration','PMOverrideCostsFlag']]:ap.remove(prop)
    if cls=='PMJunction':g.put(ap,'PMJunctionBoxTypeString','Junction Box XOR');g.put(ap,'PMJunctionBoxNumber',int(re.sub(r'\D','',ident) or '1'))
    if cls=='PMDatastore':g.put(ap,'PMDatastoreNumber',int(re.sub(r'\D','',ident) or '1'))
    if cls=='PMExternal':g.put(ap,'PMExternalNumber',int(re.sub(r'\D','',ident) or '1'))
    activities[cache]=a
   a=activities[cache];b=obj(d,'PMBox',g.find(prototype,'PMBox'));bp=props(b)
   x,y,w,h=[float(geo.get(k,'0')) for k in ['x','y','width','height']]
   setrect(bp,'PMBoxCoordinates',x,y,w,h);g.put(bp,'PMActivityRef',a.get('id'))
   num=int(re.sub(r'\D','',ident) or '0');g.put(bp,'PMPersistentBoxNumber',num);g.put(bp,'PMUowIdNumber',num)
   boxes[ident]=b;rects[ident]=(x,y,w,h)
  def endpoint(pt,explicit=None):
   if explicit in boxes:return boxes[explicit].get('id')
   xx,yy=pt
   for ident,(x,y,w,h) in rects.items():
    if x-2<=xx<=x+w+2 and y-2<=yy<=y+h+2 and min(abs(xx-x),abs(xx-x-w),abs(yy-y),abs(yy-y-h))<3:return boxes[ident].get('id')
   point=coord(xx,yy)
   if point not in allpoints:
    nd=obj(d,'PMNode');g.members(props(nd),'PMPointLocation',['x','y'],point);allpoints[point]=nd.get('id')
   return allpoints[point]
  for cell in cells:
   geo=cell.find('mxGeometry')
   if geo is None:continue
   if cell.get('vertex')=='1' and cell.get('id','').startswith('text'):
    x,y,w,h=[float(geo.get(k,'0')) for k in ['x','y','width','height']]
    if y<100 or y>1275:continue
    text=cell.get('value','');tb=obj(d,'PMTextBlock');tp=props(tb)
    g.put(tp,'PMTextString',text);setrect(tp,'PMTextBoundingBox',x,y,w+15,h+5)
    g.put(tp,'PMTextBlockFontRef',g.font);continue
   if cell.get('edge')!='1':continue
   start=geo.find("mxPoint[@as='sourcePoint']");end=geo.find("mxPoint[@as='targetPoint']")
   if start is None or end is None:continue
   points=[(float(p.get('x')),float(p.get('y'))) for p in [start]+geo.findall('./Array/mxPoint')+[end]]
   if points[0]==points[-1] or min(y for x,y in points)>=1290:continue
   source=endpoint(points[0],cell.get('source'));sink=endpoint(points[-1],cell.get('target'))
   ar=obj(model,'PMArrow');arp=props(ar);text=cell.get('dataLabel','');g.put(arp,'Name',text);ar.set('Name',text)
   for nm in ['PMNameColorReference','PMTextColorReference']:g.put(arp,nm,'')
   lab=obj(d,'PMArrowLabel');lp=props(lab);g.put(lp,'PMArrowRef',ar.get('id'));g.put(lp,'PMTextFontRef',g.font)
   # Text is retained as a native text block at the source layout position.
   g.members(lp,'PMTextBoundingBox',['left','top','right','bottom'],[0,0,0,0]);g.put(lp,'PMDrawSquiggleFlag',64)
   seg=obj(lab,'PMArrowSegment');sp=props(seg)
   for c in list(sp):
    if c.tag==T('PMVertices'):sp.remove(c)
   pts=[coord(x,y) for x,y in points];direction=2 if pts[0][0]==pts[1][0] else 1
   for i,(x,y) in enumerate(pts+[(0,direction)]):E.SubElement(sp,T('PMVertices'),index=str(i)).text=f'({x},{y})'
   g.put(sp,'PMSourceConnectionRef',source);g.put(sp,'PMSinkConnectionRef',sink)
   g.put(sp,'PMLineWidth',1);g.put(sp,'PMArrowDrawingStyleFlag',0)
  diagrams.append(d)
 ids={e.get('id') for e in root.iter() if e.get('id')}
 for e in root.iter():
  if e.text and e.text.startswith('{') and e.text not in ids:
   if 'Color' in e.tag:e.text=''
   else:raise ValueError(('Unresolved reference',e.tag,e.text))
 E.indent(root);dest=g.OUT/('SmartCoworking_'+family+'.xml');E.ElementTree(root).write(dest,encoding='utf-8',xml_declaration=True)
 manifest.append({'family':family,'xml':dest.name,'pages':[p.get('name') for p in pages]})
(g.OUT/'native_manifest.json').write_text(json.dumps(manifest,ensure_ascii=False,indent=2),encoding='utf-8')
print('Generated native XML:',[(m['family'],len(m['pages'])) for m in manifest])
