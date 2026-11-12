"""Spaced layout for the six-function IDEF0 decomposition."""
def draw_decomposition(Diagram, functions):
 d=Diagram('02_IDEF0_A0','IDEF0 — декомпозиция A0')
 positions={'A1':(200,340),'A2':(1000,340),'A3':(1800,340),
            'A6':(200,920),'A5':(1000,920),'A4':(1800,920)}
 w,h=400,180
 labels=[]
 routes=[]
 def line(points,arrow=True):
  routes.append(points)
  d.line(points,arrow=arrow)
 def label(x,y,text,size=25):labels.append((x,y,text,size))
 def junction(x,y):
  d.d.ellipse((x-5,y-5,x+5,y+5),fill='black')
  d.svg.append(f'<circle cx="{x}" cy="{y}" r="5" fill="black"/>')
  d.k+=1
  # A small filled ellipse preserves branch marks in the editable drawing.
  import xml.etree.ElementTree as X
  cell=X.SubElement(d.root,'mxCell',id='junction'+str(d.k),parent='1',vertex='1',
    style='ellipse;fillColor=black;strokeColor=black;',value='')
  X.SubElement(cell,'mxGeometry',x=str(x-5),y=str(y-5),width='10',height='10',attrib={'as':'geometry'})
 inputs={'A1':'I1, I2','A2':'I1, I2','A3':'I1','A4':'I1–I3','A5':'I2','A6':'I2'}
 outputs={'A1':'O1, O2, O4','A2':'O1, O2, O4','A3':'O1, O4',
          'A4':'O1–O4','A5':'O1, O2, O4','A6':'O2, O4'}
 # Draw the shared service trunks first, with separate vertical lanes.
 # F1 is an output of A1 and a control for every other activity.
 line([(600,390),(800,390),(800,180)],False)
 line([(800,180),(2030,180)],False)
 for x in [1230,2030]:line([(x,180),(x,340)])
 line([(800,390),(800,760)],False)
 line([(500,760),(2030,760)],False)
 for x in [500,1230,2030]:line([(x,760),(x,920)])
 label(1580,155,'F1');label(1080,735,'F1')
 # F7 comes from A6 and controls A1–A5.
 line([(600,1000),(850,1000),(850,110)],False)
 line([(500,110),(2130,110)],False)
 for x in [500,1330,2130]:line([(x,110),(x,340)])
 line([(850,680),(2130,680)],False)
 for x in [1330,2130]:line([(x,680),(x,920)])
 label(1570,85,'F7');label(2010,650,'F7')
 # F8 joins audit events and reaches the input (left) of A6.
 for x,y in [(600,465),(1400,465),(2200,465),(2200,1050),(1400,1050)]:
  lane=x+60
  line([(x,y),(lane,y),(lane,1360)],False)
 line([(2260,1360),(100,1360),(100,1060),(200,1060)])
 label(1200,1335,'F8')
 # Business links use distinct channels, and always enter the left of a box.
 line([(1400,430),(1800,430)]);label(1600,405,'F2')
 line([(2200,390),(2340,390),(2340,840),(1720,840),(1720,975),(1800,975)])
 label(2100,815,'F3')
 line([(2200,1010),(2370,1010),(2370,610),(1740,610),(1740,470),(1800,470)])
 label(2080,585,'F4')
 line([(2200,425),(2300,425),(2300,800),(930,800),(930,975),(1000,975)])
 label(1580,775,'F5')
 line([(1400,1010),(1640,1010),(1640,505),(1800,505)])
 label(1680,550,'F6')
 # Short ICOM stubs are separated from business links and from each other.
 for a,title,*_ in functions:
  x,y=positions[a]
  d.node(a,x,y,w,h,title,size=30)
  label(x+w-24,y+h-22,a[1:],26)
  extra=[]
  if a in ['A1','A2','A3','A6']:extra.append('C2')
  if a!='A2':extra.append('C4')
  controls='C1, C3'+ ('\n'+', '.join(extra) if extra else '')
  line([(x+80,y-70),(x+80,y)])
  label(x+160,y-110 if y==340 else y-62,controls,24)
  line([(x+80,y+h+70),(x+80,y+h)])
  label(x+80,y+h+119,'M1–M3',24)
  line([(x-145,y+25),(x,y+25)])
  label(x-85,y-3,inputs[a],24)
  if a=='A3':
   line([(x+w,y+h-10),(2240,y+h-10),(2240,300),(2380,300)])
   label(2310,272,outputs[a],22)
  else:
   line([(x+w,y+h-10),(x+w+140,y+h-10)])
   label(x+w+120,y+h+20,outputs[a],20)
 # Draw small bridges at crossings, so independent arrows cannot look joined.
 horizontal=[];vertical=[]
 for route in routes:
  for p,q in zip(route,route[1:]):
   if p[1]==q[1]:horizontal.append((min(p[0],q[0]),max(p[0],q[0]),p[1]))
   elif p[0]==q[0]:vertical.append((p[0],min(p[1],q[1]),max(p[1],q[1])))
 crossings={(x,y) for left,right,y in horizontal for x,top,bottom in vertical
            if left+14<x<right-14 and top+14<y<bottom-14}
 for x,y in sorted(crossings):
  d.d.line([(x-12,y),(x+12,y)],fill='white',width=9)
  d.d.line([(x,y-15),(x,y+15)],fill='black',width=3)
  d.d.arc((x-12,y-12,x+12,y+12),180,360,fill='white',width=9)
  d.d.arc((x-12,y-12,x+12,y+12),180,360,fill='black',width=3)
  d.svg.append(f'<path d="M{x-12},{y}H{x+12}" stroke="white" stroke-width="9"/>')
  d.svg.append(f'<path d="M{x},{y-15}V{y+15}" stroke="black" stroke-width="3"/>')
  path=f'M{x-12},{y}A12,12 0 0 1 {x+12},{y}'
  d.svg.append(f'<path d="{path}" fill="none" stroke="white" stroke-width="9"/>')
  d.svg.append(f'<path d="{path}" fill="none" stroke="black" stroke-width="3"/>')
 for cell in d.root.findall('mxCell'):
  if cell.get('edge')=='1':cell.set('style',cell.get('style')+';jumpStyle=arc;jumpSize=12;')
 # Dots indicate actual joins.
 for x,y in [(800,390),(1230,180),(500,760),(800,760),(1230,760),
             (850,110),(1330,110),(850,680),(1330,680),
             (660,1360),(1460,1360),(1460,1050),(2260,1050)]:junction(x,y)
 for x,y,text,size in labels:d.label(x,y,text,size)
 d.finish()
