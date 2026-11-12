$ErrorActionPreference='Stop'
function GetVal($object,$name){
 $p=$object.Properties.Item($name)
 return $p.GetType().InvokeMember('Value',[Reflection.BindingFlags]::GetProperty,$null,$p,@())
}
function PutVal($object,$name,$value){
 if($object.Properties.HasProperty($name)){$p=$object.Properties.Item($name)}else{$p=$object.Properties.Add($name)}
 $argsList=New-Object object[] 1; $argsList[0]=$value
 $p.GetType().InvokeMember('Value',[Reflection.BindingFlags]::SetProperty,$null,$p,$argsList) | Out-Null
}
$api=New-Object -ComObject ProcessModeler70.SCAPI
$bag=New-Object -ComObject ProcessModeler70.SCAPI.PropertyBag
$bag.Add('Name','Smart Coworking IDEF0') | Out-Null
$bag.Add('ModelType',0) | Out-Null
$pu=$api.PersistenceUnits.Create($bag)
$s=$api.Sessions.Add()
$s.Open($pu,0) | Out-Null
$tx=$s.BeginTransaction()
$activity=@($s.ModelObjects | Where-Object ClassName -eq PMActivity)[0]
PutVal $activity 'Name' 'Обеспечить работу смарт-коворкинга'
Write-Output (GetVal $activity 'Name')
$diagram=@($s.ModelObjects | Where-Object ClassName -eq PMDiagram)[0]
$box=@($s.ModelObjects | Where-Object ClassName -eq PMBox)[0]
$font=GetVal $diagram 'PMDefaultArrowFontRef'
PutVal $box 'PMBoxCoordinates' ([int[]]@(400,210,720,390))
function AddChild($parent,$class){
 Write-Host ('AddChild '+$class); $col=$s.ModelObjects.Collect($parent,[Type]::Missing,1)
 return $col.Add($class)
}
function Arrow($name,$source,$sink,$points,$rect){
 $a=AddChild $s.ModelObjects.Root 'PMArrow'; PutVal $a 'Name' $name
 $label=AddChild $diagram 'PMArrowLabel'
 PutVal $label 'PMArrowRef' $a.ObjectId
 PutVal $label 'PMTextFontRef' $font
 PutVal $label 'PMLabelConnectPoint' ([int[]]@(0,0))
 PutVal $label 'PMSegmentConnectPoint' ([int[]]@(0,0))
 PutVal $label 'PMDrawSquiggleFlag' ([byte]64)
 PutVal $label 'PMTextBoundingBox' ([int[]]$rect)
 $seg=AddChild $label 'PMArrowSegment'
 PutVal $seg 'PMSourceConnectionRef' $source
 PutVal $seg 'PMSinkConnectionRef' $sink
 PutVal $seg 'PMLineWidth' ([byte]1)
 PutVal $seg 'PMSourceTunnelStyleFlag' ([byte]0)
 PutVal $seg 'PMSinkTunnelStyleFlag' ([byte]0)
 PutVal $seg 'PMLabeledSegment' 0
 PutVal $seg 'PMArrowDrawingStyleFlag' ([byte]0)
 $vertices=New-Object 'int[,]' ($points.Count+1),2
 for($i=0;$i -lt $points.Count;$i++){$vertices[$i,0]=$points[$i][0];$vertices[$i,1]=$points[$i][1]}
 $vertices[$points.Count,0]=0; $vertices[$points.Count,1]=1
 PutVal $seg 'PMVertices' $vertices
}
$left=GetVal $diagram 'PMLeftRef'; $right=GetVal $diagram 'PMRightRef'
$top=GetVal $diagram 'PMTopRef'; $bottom=GetVal $diagram 'PMBottomRef'
$names=@('I1 Обращения клиента','I2 Обращения администратора','I3 Ответы платёжного сервиса')
for($i=0;$i -lt 3;$i++){$y=245+$i*55; Arrow $names[$i] $left $box.ObjectId @(@(0,$y),@(400,$y)) @(25,($y-30),375,($y-5))}
$names=@('O1 Ответы клиенту','O2 Ответы администратору','O3 Запросы платёжному сервису','O4 Согласованные записи и аудит')
for($i=0;$i -lt 4;$i++){$y=230+$i*45; Arrow $names[$i] $box.ObjectId $right @(@(720,$y),@(1109,$y)) @(745,($y-30),1090,($y-5))}
$names=@('C1 Правила бронирования','C2 Настройки и тарифы','C3 Политика безопасности','C4 Серверное время')
for($i=0;$i -lt 4;$i++){$x=435+$i*80; $yy=35+($i%2)*55; Arrow $names[$i] $top $box.ObjectId @(@($x,0),@($x,210)) @(($x-75),$yy,($x+100),($yy+35))}
$names=@('M1 Клиент и администратор','M2 Веб-интерфейсы и сервер','M3 Хранилище и средства ввода')
for($i=0;$i -lt 3;$i++){$x=440+$i*110; $yy=465+($i%2)*65; Arrow $names[$i] $bottom $box.ObjectId @(@($x,615),@($x,390)) @(($x-100),$yy,($x+100),($yy+40))}
Write-Host 'COMMIT START'; $s.CommitTransaction($tx) | Write-Output
$outPath=Join-Path $PSScriptRoot '../исходники/ProcessModeler'
New-Item -ItemType Directory -Force $outPath | Out-Null
$pu.Save((Join-Path $outPath 'SmartCoworking_IDEF0.bp1')) | Write-Output
$s.Close() | Out-Null






