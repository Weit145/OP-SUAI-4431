from pathlib import Path
from PIL import Image, ImageDraw, ImageFont
import xml.etree.ElementTree as X
from html import escape
from model import *
import math, textwrap, json
OUT=Path(__file__).resolve().parent; ASSETS=OUT/'диаграммы';ASSETS.mkdir(exist_ok=True)
FONT='C:/Windows/Fonts/times.ttf' if Path('C:/Windows/Fonts/times.ttf').exists() else '/usr/share/fonts/truetype/croscore/Tinos-Regular.ttf'
BOLD='C:/Windows/Fonts/timesbd.ttf' if FONT.startswith('C:') else FONT.replace('Regular','Bold')
book=X.Element('mxfile',host='app.diagrams.net',agent='ЛР3 Смарт-коворкинг'); MANIFEST=[]
class Diagram:
 def __init__(self,key,title,legend=''):
  self.key=key;self.title=title;self.nodes={};self.kinds={};self.k=0
  self.im=Image.new('RGB',(2400,1500),'white');self.d=ImageDraw.Draw(self.im)
  self.svg=['<svg xmlns="http://www.w3.org/2000/svg" width="2400" height="1500" viewBox="0 0 2400 1500"><rect width="2400" height="1500" fill="white"/>']
  page=X.SubElement(book,'diagram',name=key,id=key);model=X.SubElement(page,'mxGraphModel',page='1',pageWidth='2400',pageHeight='1500',grid='1');self.root=X.SubElement(model,'root');X.SubElement(self.root,'mxCell',id='0');X.SubElement(self.root,'mxCell',id='1',parent='0')
  # Captions and explanations belong to the report, outside the diagram.
 def label(self,x,y,s,size=30,bold=False,align='center',width=None,editable=True):
  font=ImageFont.truetype(BOLD if bold else FONT,size)
  lines=[]
  for para in str(s).split('\n'):
   if not width:lines.append(para);continue
   line=''
   for word in para.split():
    q=(line+' '+word).strip()
    if font.getlength(q)>width and line:lines.append(line);line=word
    else:line=q
   lines.append(line)
  if editable:
   self.k+=1
   c=X.SubElement(self.root,'mxCell',id='text'+str(self.k),parent='1',vertex='1',value='\n'.join(lines),style=f'text;html=0;whiteSpace=wrap;align={align if align=="center" else "left"};verticalAlign=top;fillColor=white;strokeColor=none;fontFamily=Times New Roman;fontSize={size};fontStyle={1 if bold else 0};')
   tw=max([font.getlength(t) for t in lines] or [1])+12
   X.SubElement(c,'mxGeometry',x=str(x-tw/2 if align=='center' else x),y=str(y-size/2),width=str(tw),height=str(len(lines)*(size+5)),attrib={'as':'geometry'})
  for i,line in enumerate(lines):
   yy=y+i*(size+5); anchor='mm' if align=='center' else 'lm'
   bbox=self.d.textbbox((x,yy),line,font=font,anchor=anchor)
   self.d.rectangle((bbox[0]-3,bbox[1]-2,bbox[2]+3,bbox[3]+2),fill='white')
   self.svg.append(f'<rect x="{bbox[0]-3}" y="{bbox[1]-2}" width="{bbox[2]-bbox[0]+6}" height="{bbox[3]-bbox[1]+4}" fill="white"/>')
   self.d.text((x,yy),line,font=font,fill='#111',anchor=anchor)
   self.svg.append(f'<text x="{x}" y="{yy+size*.32}" text-anchor="{"middle" if align=="center" else "start"}" font-family="Times New Roman,serif" font-size="{size}" font-weight="{"bold" if bold else "normal"}">{escape(line)}</text>')
 def line(self,pts,label='',dashed=False,dot=False,arrow=True,optional=False,both=False):
  col='#222'; flat=[tuple(p) for p in pts]
  if dashed:
   for a,b in zip(flat,flat[1:]):
    length=math.dist(a,b);n=max(1,int(length/15))
    for i in range(0,n,2):
     f=i/n;g=min(i+1,n)/n;self.d.line([(a[0]+(b[0]-a[0])*f,a[1]+(b[1]-a[1])*f),(a[0]+(b[0]-a[0])*g,a[1]+(b[1]-a[1])*g)],fill=col,width=3)
  else:self.d.line(flat,fill=col,width=3)
  path='M'+' L'.join(f'{x},{y}' for x,y in flat)
  self.svg.append(f'<path d="{path}" fill="none" stroke="{col}" stroke-width="3"'+(' stroke-dasharray="10 7"' if dashed else '')+'/>')
  if arrow:
   a,b=flat[-2:];ang=math.atan2(b[1]-a[1],b[0]-a[0]);p=[b,(b[0]-17*math.cos(ang-.45),b[1]-17*math.sin(ang-.45)),(b[0]-17*math.cos(ang+.45),b[1]-17*math.sin(ang+.45))]
   self.d.polygon(p,fill=col);self.svg.append('<polygon points="'+' '.join(f'{x},{y}' for x,y in p)+'" fill="#222"/>')
  if both:
   a,b=flat[1],flat[0];ang=math.atan2(b[1]-a[1],b[0]-a[0]);tri=[b,(b[0]-17*math.cos(ang-.45),b[1]-17*math.sin(ang-.45)),(b[0]-17*math.cos(ang+.45),b[1]-17*math.sin(ang+.45))]
   self.d.polygon(tri,fill=col);self.svg.append('<polygon points="'+' '.join(f'{x},{y}' for x,y in tri)+'" fill="#222"/>')
  if dot:
   x,y=flat[-1];self.d.ellipse((x-7,y-7,x+7,y+7),fill=col);self.svg.append(f'<circle cx="{x}" cy="{y}" r="7" fill="#222"/>')
  if optional:
   x,y=flat[0];p=[(x-10,y),(x,y-10),(x+10,y),(x,y+10)];self.d.polygon(p,fill='white',outline=col,width=2);self.svg.append('<polygon points="'+' '.join(f'{a},{b}' for a,b in p)+'" fill="white" stroke="#222" stroke-width="2"/>')
  if label:
   a,b=max(zip(flat,flat[1:]),key=lambda ab:math.dist(*ab));lw=min(520,max(160,abs(a[0]-b[0])-25));lh=max(1,math.ceil(ImageFont.truetype(FONT,26).getlength(label)/lw));self.label((a[0]+b[0])/2,(a[1]+b[1])/2-20-(lh-1)*16,label,26,width=lw)
  self.k+=1;cell=X.SubElement(self.root,'mxCell',id='e'+str(self.k),parent='1',edge='1',value='',dataLabel=label,style='edgeStyle=none;html=1;fontFamily=Times New Roman;fontSize=26;endArrow='+('oval;endFill=1' if dot else 'block' if arrow else 'none')+(';dashed=1' if dashed else '')+(';startArrow=diamond;startFill=0' if optional else ';startArrow=block' if both else ''))
  g=X.SubElement(cell,'mxGeometry',relative='1',attrib={'as':'geometry'});X.SubElement(g,'mxPoint',x=str(flat[0][0]),y=str(flat[0][1]),attrib={'as':'sourcePoint'});X.SubElement(g,'mxPoint',x=str(flat[-1][0]),y=str(flat[-1][1]),attrib={'as':'targetPoint'})
  ar=X.SubElement(g,'Array',attrib={'as':'points'})
  for x,y in flat[1:-1]:X.SubElement(ar,'mxPoint',x=str(x),y=str(y))
  return cell
 def node(self,id,x,y,w,h,s,kind='box',fill='white',size=30):
  uob_number=None
  if 'IDEF3' in self.key and id.startswith('U') and '\n' in s:
   uob_number,s=s.split('\n',1)
  self.nodes[id]=(x,y,w,h);self.kinds[id]=kind
  radius=20 if kind in ['process','dependent'] else 0
  if kind=='store':
   code,name=s.split(' ',1) if ' ' in s else (s,'')
   divider=76
   style='shape=partialRectangle;top=1;bottom=1;left=1;right=0;whiteSpace=wrap;html=0;fillColor=white;strokeColor=black;'
   c=X.SubElement(self.root,'mxCell',id=id,parent='1',vertex='1',value='',style=style,dataStoreCode=code,dataStoreName=name)
   X.SubElement(c,'mxGeometry',x=str(x),y=str(y),width=str(w),height=str(h),attrib={'as':'geometry'})
   self.d.line([(x,y),(x+w,y)],fill='black',width=3);self.d.line([(x,y+h),(x+w,y+h)],fill='black',width=3)
   self.d.line([(x,y),(x,y+h)],fill='black',width=3)
   self.svg.append(f'<path d="M{x},{y}h{w} M{x},{y+h}h{w} M{x},{y}v{h}" fill="none" stroke="black" stroke-width="3"/>')
   self.line([(x+divider,y),(x+divider,y+h)],arrow=False)
   self.label(x+divider/2,y+h/2,code,size)
   font=ImageFont.truetype(FONT,size)
   lines=[];line=''
   for word in name.split():
    candidate=(line+' '+word).strip()
    if line and font.getlength(candidate)>w-divider-32:lines.append(line);line=word
    else:line=candidate
   if line:lines.append(line)
   yy=y+h/2-(len(lines)-1)*(size+5)/2
   self.label(x+divider+(w-divider)/2,yy,'\n'.join(lines),size)
   return
  else:
   self.d.rounded_rectangle((x,y,x+w,y+h),radius=radius,fill=fill,outline='#222',width=3)
   self.svg.append(f'<rect x="{x}" y="{y}" width="{w}" height="{h}" rx="{radius}" fill="{fill}" stroke="#222" stroke-width="3"/>')
  lines=[]
  font=ImageFont.truetype(FONT,size)
  for raw in s.split('\n'):
   line=''
   for word in raw.split():
    q=(line+' '+word).strip()
    if font.getlength(q)>w-40 and line:lines.append(line);line=word
    else:line=q
   lines.append(line)
  yy=y+h/2-(len(lines)-1)*(size+5)/2
  self.label(x+w/2,yy,'\n'.join(lines),size,editable=False)
  style='shape=partialRectangle;top=1;bottom=1;left=1;right=0;' if kind=='store' else 'rounded=1;' if radius else 'rounded=0;'
  c=X.SubElement(self.root,'mxCell',id=id,parent='1',vertex='1',value=s,style=style+f'whiteSpace=wrap;html=0;fontFamily=Times New Roman;fontSize={size};fillColor={fill};strokeColor=#222222;')
  X.SubElement(c,'mxGeometry',x=str(x),y=str(y),width=str(w),height=str(h),attrib={'as':'geometry'})
  if uob_number:
   self.line([(x,y+h-42),(x+w,y+h-42)],arrow=False)
   self.label(x+w-85,y+h-20,uob_number,23)
 def point(self,id,side):
  x,y,w,h=self.nodes[id];return {'L':(x,y+h/2),'R':(x+w,y+h/2),'T':(x+w/2,y),'B':(x+w/2,y+h)}[side]
 def edge(self,a,b,label='',sa='R',sb='L',via=None,**kwargs):
  start=self.point(a,sa);end=self.point(b,sb)
  if via is None and start[0]!=end[0] and start[1]!=end[1]:
   if sa in ['L','R']:
    mid=(start[0]+end[0])/2;via=[(mid,start[1]),(mid,end[1])]
   else:
    mid=(start[1]+end[1])/2;via=[(start[0],mid),(end[0],mid)]
  pts=[start]+(via or [])+[end]
  if 'DFD' in self.key:
   if self.kinds[b]=='store' and (('чтение' in label.lower() and 'запись' not in label.lower()) or label=='Состояние / срок'):pts=list(reversed(pts))
   elif ' / ' in label or '/ F' in label or label=='Задания / выборка':kwargs['both']=True
  cell=self.line(pts,label,**kwargs)
  reverse=pts[0]!=self.point(a,sa)
  cell.set('source',b if reverse else a);cell.set('target',a if reverse else b)
 def finish(self):
  self.svg.append('</svg>');(ASSETS/(self.key+'.svg')).write_text(''.join(self.svg));self.im.save(ASSETS/(self.key+'.png'))
  MANIFEST.append({'key':self.key,'title':self.title,'png':self.key+'.png'})

def entity(d,name,x,y,w=650,h=470):
 row=E[name];keys=row[2][:];attrs=row[3][:];dep=any('[FK]' in k for k in keys)
 for parent,child,fk,*_ in RELATIONS:
  if child==name:
   keys=[v.replace('[FK]', '[FK→'+parent+']') if v.split()[0].rstrip('?')==fk else v for v in keys]
   attrs=[v.replace('[FK]', '[FK→'+parent+']') if v.split()[0].rstrip('?')==fk else v for v in attrs]
 d.node(name,x,y,w,h,'','dependent' if dep else 'box',size=28)
 d.label(x+w/2,y+32,name,30,True)
 d.label(x+22,y+80,'\n'.join(keys),26,align='left')
 sep=y+88+len(keys)*31;d.line([(x,sep),(x+w,sep)],arrow=False)
 d.label(x+22,sep+27,'\n'.join(attrs or ['(неключевых атрибутов нет)']),25,align='left')
 # Replace empty editable entity with complete text and separator.
 cell=next(c for c in d.root.findall('mxCell') if c.get('id')==name)
 cell.set('value','')
 cell.set('style',cell.get('style')+'align=left;verticalAlign=top;spacing=16;')

from revised import configure
configure(globals())

if __name__=='__main__':
 context();dfd();er_diagrams()
 for i,s in enumerate(SCENARIOS):scenario_diagram(i,s)
 X.indent(book);X.ElementTree(book).write(OUT/'ЛР3_Смарт-коворкинг.drawio',encoding='utf-8',xml_declaration=True)
 (OUT/'manifest.json').write_text(json.dumps(MANIFEST,ensure_ascii=False,indent=2))
 print('Диаграмм:',len(MANIFEST))
