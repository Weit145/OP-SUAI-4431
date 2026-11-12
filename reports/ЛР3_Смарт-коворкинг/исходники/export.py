from pathlib import Path
import subprocess, shutil, time, uno
from com.sun.star.beans import PropertyValue
BASE=Path(__file__).resolve().parent.parent
SOURCE=BASE/'ЛР3_Смарт-коворкинг_Кочнев_4431.docx'
def pv(k,v):p=PropertyValue();p.Name=k;p.Value=v;return p
shutil.copy2(SOURCE,'/tmp/lr3_input.docx')
proc=subprocess.Popen(['libreoffice','-env:UserInstallation=file:///tmp/lo_lr3_export','--headless','--accept=socket,host=127.0.0.1,port=2013;urp;StarOffice.ComponentContext','--norestore','--nodefault','--nofirststartwizard'])
try:
 local=uno.getComponentContext();resolver=local.ServiceManager.createInstanceWithContext('com.sun.star.bridge.UnoUrlResolver',local)
 for _ in range(80):
  try:ctx=resolver.resolve('uno:socket,host=127.0.0.1,port=2013;urp;StarOffice.ComponentContext');break
  except Exception:time.sleep(.25)
 desktop=ctx.ServiceManager.createInstanceWithContext('com.sun.star.frame.Desktop',ctx)
 doc=desktop.loadComponentFromURL('file:///tmp/lr3_input.docx','_blank',0,(pv('Hidden',True),))
 assert doc is not None
 indexes=doc.getDocumentIndexes();print('Indexes:',indexes.Count,flush=True)
 if indexes.Count==0:
  ds=doc.createSearchDescriptor();ds.SearchString='СОДЕРЖАНИЕ';hit=doc.findFirst(ds);cursor=doc.Text.createTextCursorByRange(hit.End);cursor.gotoNextParagraph(False)
  index=doc.createInstance('com.sun.star.text.ContentIndex');index.CreateFromOutline=True;index.Level=2;index.Title='';doc.Text.insertTextContent(cursor,index,False)
 for _ in range(2):
  for i in range(doc.DocumentIndexes.Count):doc.DocumentIndexes.getByIndex(i).update()
 doc.storeToURL('file:///tmp/lr3_output.docx',(pv('FilterName','Office Open XML Text'),pv('Overwrite',True)))
 doc.storeToURL('file:///tmp/lr3_output.pdf',(pv('FilterName','writer_pdf_Export'),pv('Overwrite',True)))
 print('Pages:',doc.CurrentController.PageCount,'Tables:',doc.TextTables.Count,'Images:',doc.GraphicObjects.Count,flush=True)
 shutil.copy2('/tmp/lr3_output.docx',SOURCE);shutil.copy2('/tmp/lr3_output.pdf',SOURCE.with_suffix('.pdf'))
 doc.close(True);desktop.terminate()
finally:proc.terminate()
