"""Revised layouts; uses the same semantic model as the report."""
import re
from model import FLOWS, STORES

def configure(g):
 overrides=dict(context=context,dfd=dfd,er_diagrams=er_diagrams,scenario_diagram=scenario_diagram)
 globals().update({k:v for k,v in g.items() if not k.startswith('__') and k not in overrides})
 g.update(overrides)

def context():
 d=Diagram('01_IDEF0_A-0','IDEF0 — контекст A-0','Входы — слева; управления — сверху; результаты — справа; механизмы — снизу. Декомпозиция блока 0: A0.')
 d.node('A0',850,500,720,340,'Обеспечить бронирование и обслуживание клиентов смарт-коворкинга\n0',size=38)
 for i,(c,t) in enumerate([('I1','Обращения клиента'),('I2','Обращения администратора'),('I3','Ответы платёжного сервиса')]):
  y=565+i*100;d.line([(50,y),(850,y)]);d.label(435,y-28,c+' '+t,30)
 for i,(c,t) in enumerate([('C1','Бизнес-правила'),('C2','Настройки и тарифы'),('C3','Безопасность'),('C4','Время UTC+3')]):
  x=930+i*180;d.line([(x,280+i%2*75),(x,500)]);d.label(x,180+i%2*75,c+'\n'+t,28,width=220)
 for i,(c,t) in enumerate([('O1','Ответы клиенту'),('O2','Ответы администратору'),('O3','Запросы платёжному сервису'),('O4','Записи и аудит')]):
  y=535+i*85;d.line([(1570,y),(2350,y)]);d.label(1960,y-28,c+' '+t,29)
 for i,(c,t) in enumerate([('M1','Клиент и администратор'),('M2','Веб-интерфейсы и сервер'),('M3','Хранилище, сканер QR')]):
  x=960+i*240;d.line([(x,1060),(x,840)]);d.label(x,1110+i%2*85,c+'\n'+t,29,width=300)
 d.finish()
 d=Diagram('02_IDEF0_A0','IDEF0 — декомпозиция A0','F1: доступ; F2: предложение; F3/F4: финансовый обмен; F5/F6: QR и допуск; F7: контроль; F8: события. ICOM — см. словарь.')
 pos={'A1':(150,360),'A2':(900,360),'A3':(1650,360),'A4':(1650,920),'A5':(900,920),'A6':(150,920)}
 for a,title,*_ in FUNCTIONS:
  x,y=pos[a];d.node(a,x,y,560,190,title+'\n'+a,size=32)
  codes='C1,C3'+(',C2' if a in ['A1','A2','A3','A6'] else '')+(',C4' if a!='A2' else '')
  d.line([(x+80,y-95),(x+80,y)],codes)
  d.line([(x+80,y+290),(x+80,y+190)],'M1–M3')
  inp='I1, I2' if a in ['A1','A2','A4'] else 'I1' if a=='A3' else 'I2'
  d.line([(x-110,y+35),(x,y+35)],inp)
  out='O2,O4' if a=='A6' else 'O1,O4' if a=='A3' else 'O1,O2,O3,O4' if a=='A4' else 'O1,O2,O4'
  d.line([(x+560,y+160),(x+665,y+160)],out)
 # Access context: the top activity feeds a common branch trunk in the middle.
 d.line([(710,435),(765,435),(765,700),(2320,700)],arrow=False)
 d.line([(420,700),(765,700)],arrow=False)
 d.label(1150,680,'F1 — Проверенный контекст доступа',26)
 for a,xx in [('A2',1110),('A3',1860)]:
  x,y=pos[a];lane=x-85;d.line([(lane,700),(lane,220),(xx,220),(xx,y)])
 for a,xx in [('A6',420),('A5',1170),('A4',1920)]:d.line([(xx,700),(xx,920)])
 # Control from A6, independently of data context.
 d.line([(710,995),(795,995),(795,625),(2340,625)],arrow=False)
 d.line([(65,625),(795,625)],arrow=False)
 d.label(1590,605,'F7 — Контрольное воздействие',26)
 for a,xx,lane in [('A1',580,65),('A2',1330,825),('A3',2080,1580)]:
  d.line([(lane,625),(lane,175),(xx,175),(xx,360)])
 for xx in [1370,2120]:d.line([(xx,625),(xx,920)])
 # Explicit source-to-target business exchanges, each with its own lane.
 d.line([(1460,435),(1650,435)],'F2')
 d.line([(2210,450),(2290,450),(2290,835),(1720,835),(1720,920)],'F3')
 d.line([(2210,1010),(2310,1010),(2310,575),(1590,575),(1590,485),(1650,485)],'F4')
 d.line([(2210,480),(2270,480),(2270,790),(980,790),(980,920)],'F5')
 d.line([(1460,990),(1530,990),(1530,510),(1650,510)],'F6')
 # Audit/control records join below the blocks; every producer is represented.
 for a,xx in [('A1',740),('A2',1500),('A3',2350),('A4',2350),('A5',1500)]:
  x,y=pos[a];d.line([(x+560,y+110),(xx,y+110),(xx,1250)],arrow=False)
 d.line([(2350,1250),(95,1250),(95,1030),(150,1030)],'F8 — События аудита и контроля')
 d.line([(1540,990),(1650,990)],'I3')
 d.finish()

# All DFD edges are explicit directed data flows. Repeated P/D symbols denote
# the same objects on coordinated sheets, not extra processes/stores.
def expanded_codes(s):
 s=s.replace('–','-')
 def expand(m):return ', '.join(m[1]+str(i) for i in range(int(m[2]),int(m[3])+1))
 return re.sub(r'([PAD])([1-6])-\1?([1-8])',expand,s).split(', ')

def dfd_edges():
 edges=[]
 for code,direction,content,_ in FLOWS:
  a,b=direction.split(' → ')
  for source in expanded_codes(a):
   for target in expanded_codes(b):edges.append((source,target,code,content))
 for code,name,tables,writers,readers in STORES:
  for p in expanded_codes(writers):
   if not any(a==p and b==code for a,b,*_ in edges):edges.append((p,code,'','Записи: '+name.lower()))
  for p in expanded_codes(readers):edges.append((code,p,'','Данные: '+name.lower()))
 return edges

def dfd():
 edges=dfd_edges()
 names={'E1':'Клиент','E2':'Администратор','E3':'Платёжный сервис'}
 names.update({p:t for a,t,p,*_ in FUNCTIONS});names.update({c:n for c,n,*_ in STORES})
 d=Diagram('03_DFD_context','DFD — контекст системы P0','Внешние потоки сохраняются при декомпозиции. Коды Fxx совпадают со словарём; направления показаны отдельно.')
 d.node('P0',940,170,520,1080,'0\nИС «Смарт-коворкинг»','process',size=40)
 for j,e in enumerate(['E1','E2','E3']):
  y=240+j*350;d.node(e,70,y,500,180,e+'\n'+names[e],size=35)
  ins=[c for a,b,c,_ in edges if a==e];outs=[c for a,b,c,_ in edges if b==e]
  d.line([(570,y+50),(940,y+50)],', '.join(ins))
  d.line([(1460,y+140),(1770,y+140),(1770,y+230),(320,y+230),(320,y+180)],', '.join(outs))
  d.label(1950,y+55,{'E1':'Учётная запись, поиск,\nбронь и оплата','E2':'Настройки, QR,\nсверка возврата','E3':'Запросы и результаты\nфинансовых операций'}[e],29,width=620)
 d.finish()
 keys=['04_DFD_P1','05_DFD_P2','06_DFD_P3','06b_DFD_P4','06c_DFD_P5','06d_DFD_P6']
 for i,p in enumerate([f'P{i}' for i in range(1,7)]):
  d=Diagram(keys[i],'DFD, уровень 1 — '+p+' '+names[p],'Каждый лист — фрагмент общей декомпозиции P0. Повторённые P/E/D обозначают те же объекты. Чтение и запись разделены.')
  incoming=[(a,c,t) for a,b,c,t in edges if b==p];outgoing=[(b,c,t) for a,b,c,t in edges if a==p]
  d.node(p,990,170,430,1080,p+'\n'+names[p],'process',size=36)
  for side,items in [('L',incoming),('R',outgoing)]:
   gap=1060/max(len(items),1)
   for n,(ref,code,content) in enumerate(items):
    yy=190+gap*(n+.5);x=60 if side=='L' else 1890
    rid=side+str(n);kind='store' if ref.startswith('D') else 'process' if ref.startswith('P') else 'box'
    d.node(rid,x,yy-38,420,76,ref+' '+names[ref],kind,'#f2f2f2' if ref.startswith('P') else 'white',size=28)
    pts=[(480,yy),(990,yy)] if side=='L' else [(1420,yy),(1890,yy)]
    cell=d.line(pts)
    cell.set('source',rid if side=='L' else p);cell.set('target',p if side=='L' else rid)
    cell.set('flowCode',code);cell.set('sourceCode',ref if side=='L' else p);cell.set('targetCode',p if side=='L' else ref)
    d.label(735 if side=='L' else 1655,yy-42,(code+' ' if code else '')+content,25,width=450)
  d.finish()

# ER views use a simple fan-out layout. Referenced parents from other sheets
# are included with key-only attributes, so no FK relation is hidden in prose.
def er_diagrams():
 groups=[('07_IDEF1X_identity','Пользователи и роли',['USER_ACCOUNT','ROLE','USER_ROLE','USER_SESSION']),('08_IDEF1X_catalog','Каталог и версии тарифов',['SPACE_TYPE','RENT_RULE_VERSION','TARIFF_VERSION','SPACE','SPACE_TARIFF']),('09_IDEF1X_booking','Бронирование и история',['BOOKING','HOLD','BOOKING_EVENT','QR_PASS','VISIT']),('10_IDEF1X_finance','Финансовые операции',['BOOKING','PAYMENT_ATTEMPT','PROVIDER_EVENT','REFUND']),('11_IDEF1X_control','Хранение, задания и аудит',['USER_ACCOUNT','RETENTION_POLICY','DELETION_REQUEST','CONTROL_TASK','AUDIT_EVENT'])]
 # Separate lanes for edges, keeping attributes readable and links outside boxes.
 for key,title,names in groups:
  d=Diagram(key,'IDEF1X — '+title,'PK — над чертой; FK — внешний ключ; ? — NULL. Сплошная связь — ID; пунктир — NID; точка — потомок; Z — 0..1.')
  positions=[(80,220),(900,220),(1720,220),(80,800),(1190,800)]
  if 'identity' in key:positions=[(80,220),(1720,220),(900,800),(80,800)]
  for name,(x,y) in zip(names,positions):
   row=E[name];h=max(220,160+34*(len(row[2])+len(row[3])))
   entity(d,name,x,y,590,h)
  lane=0
  for parent,child,fk,kind,card,pc in RELATIONS:
   if parent not in names or child not in names:continue
   a=d.nodes[parent];b=d.nodes[child];lane+=1
   if a[1]==b[1]:
    if abs(a[0]-b[0])>1500:
     sa=sb='T';y=150+lane*8;via=[(a[0]+295,y),(b[0]+295,y)]
    else:sa,sb=('R','L') if a[0]<b[0] else ('L','R');via=None
   else:
    sa,sb='B','T';mid=760+lane*6;via=[(a[0]+295,mid),(b[0]+295,mid)]
   d.edge(parent,child,('Z: ' if card=='0..1' else '')+fk,sa,sb,via,dashed=kind=='NID',dot=True,arrow=False,optional=pc=='0..1')
  d.finish()

# Left-to-right scenarios with explicit decisions and independent error exits.
def scenario_diagram(idx,scenario):
 code,title,*_=scenario
 d=Diagram(f'{12+idx:02}_IDEF3_{code}','IDEF3 '+code+' — '+title,'UOB — работа; стрелка — предшествование; X — исключающее ветвление. J — номер перекрёстка; условия указаны на ветвях.')
 def u(n,x,y,text,w=560,h=180):d.node('U'+str(n),x,y,w,h,code+'.'+str(n)+'\n'+text,size=32)
 def j(n,x,y):
  d.node('J'+str(n),x,y,70,70,'X',size=38);d.label(x+35,y-25,'J'+str(n),26)
 def e(a,b,t='',sa='R',sb='L',via=None):d.edge(a,b,t,sa,sb,via)
 if code in ['S3','S6']:
  if code=='S3':
   u(1,70,170,'Создать попытку с ключом; запросить оплату вне транзакции')
   u(2,820,170,'Принять отдельное уведомление; проверить подпись и уникальность')
   j(1,1590,225);e('U1','U2','Запуск оплаты');e('U2','J1')
   u(4,1810,140,'Отклонить подделку или вернуть сохранённый результат',w=480,h=220)
   e('J1','U4','B1: подделка / дубль')
   u(5,1810,510,'Сохранить отказ; при неизвестном исходе поставить сверку',w=480,h=220)
   e('J1','U5','B2: нет списания','B','L',[(1625,620)])
   u(3,70,850,'Новое списание: SPACE → BOOKING; проверить сумму, HOLD и now < deadline',h=250)
   e('J1','U3','Подлинное новое списание','B','T',[(1625,760),(350,760)])
   j(2,820,940);e('U3','J2')
   u(6,1060,830,'Подтвердить бронь, снять HOLD; создать один QR',w=580,h=210)
   u(7,1740,1060,'Позднее / лишнее списание: полный возврат фактической суммы',w=580,h=210)
   e('J2','U6','B3: все проверки успешны')
   e('J2','U7','B4: иначе','B','L',[(855,1165)])
  else:
   u(1,70,170,'Принять QR администратора или событие конца аренды')
   u(2,820,170,'Заблокировать SPACE → BOOKING; перечитать оплату, QR, VISIT и время')
   e('U1','U2');j(1,1590,225);e('U2','J1')
   u(3,1810,140,'Проверить роль, токен, оплату, статус и now < ends_at',w=480,h=220)
   e('J1','U3','Сканирование QR')
   j(2,2015,480);e('U3','J2',sa='B',sb='T')
   u(4,1740,650,'B1: VISIT отсутствует и все проверки успешны — создать VISIT',w=580,h=220)
   u(5,1740,1050,'B2: иначе — отказ с причиной; посещение не создавать',w=580,h=210)
   e('J2','U4','Успешно','B','T');e('J2','U5','Отказ','R','R',[(2340,515),(2340,1155)])
   u(6,70,650,'При now ≥ ends_at проверить наличие VISIT',w=560,h=200)
   e('J1','U6','Конец аренды','B','T',[(1625,440),(350,440)])
   j(3,820,715);e('U6','J3')
   u(7,1060,650,'B3: завершить бронь и закрыть действие QR',w=560,h=200)
   u(8,1060,1050,'B4: неявка; отозвать QR; возврата нет',w=560,h=210)
   e('J3','U7','VISIT есть');e('J3','U8','VISIT нет','B','L',[(855,1155)])
  d.finish();return
 specs={
 'S1':(['Принять регистрацию, вход либо выход из сессии','Проверить поля и email / хеш и состояние аккаунта / сессию'],['Создать аккаунт и роль / выдать или отозвать сессию','Показать безопасное сообщение об ошибке'],'Данные допустимы','Проверка не пройдена','Сохранить результат и аудит'),
 'S2':(['Проверить фильтры; выбрать слот, тариф и стоимость','SPACE → BOOKING: проверить время и отсутствие пересечений'],['Создать BOOKING, HOLD и задание исходного дедлайна','Отказать; вернуть актуальные свободные варианты'],'Слот допустим и свободен','Иначе','Показать владельцу результат и исходный остаток времени'),
 'S4':(['Принять задание дедлайна либо явный отказ клиента','SPACE → BOOKING: перечитать состояние и срок'],['Снять действующее HOLD; записать истечение / отказ','Сохранить текущее состояние; раннее задание перенести'],'Ожидает оплаты; срок / отказ','Иное состояние / срок не настал','Завершить обработку; сверить незавершённые платежи'),
 'S5':(['Проверить владельца; показать сумму полного возврата','Получить подтверждение; под блокировкой повторить now < starts_at'],['Отменить бронь; отозвать QR; освободить слот; запросить возврат','Отказать в отмене; показать причину'],'Подтверждено; срок допустим','Иначе','Сохранить отказ / сверять возврат до окончательного результата'),
 'S7':(['Проверить роль; принять тариф, аренду или срок хранения','Проверить полноту, положительные значения и min ≤ max'],['Опубликовать новую версию / политику хранения','Отклонить изменения; показать ошибки'],'Все проверки пройдены','Иначе','Записать аудит; условия прежних броней сохранить'),
 'S8':(['Подтвердить запрос владельца; отключить вход и сессии','В задании срока проверить политику и незавершённые обязательства'],['Удалить / обезличить допустимые данные','Сохранить необходимые записи; перенести очистку'],'Срок наступил; обязательств нет','Иначе','Записать аудит; завершить либо перенести задание')}
 common,branches,yes,no,end=specs[code]
 u(1,80,200,common[0],w=600,h=220);u(2,870,200,common[1],w=670,h=220);e('U1','U2')
 j(1,1730,275);e('U2','J1')
 u(3,1570,650,branches[0],w=740,h=230)
 u(4,1570,1040,branches[1],w=740,h=210)
 e('J1','U3',yes,'B','T',[(1765,520),(1940,520)])
 e('J1','U4',no,'R','R',[(2340,310),(2340,1145)])
 j(2,1250,935)
 e('U3','J2',sa='L',sb='T',via=[(1285,765)])
 e('U4','J2',sa='L',sb='B',via=[(1285,1145)])
 u(5,80,850,end,w=930,h=240);e('J2','U5',sa='L',sb='R')
 d.finish()
