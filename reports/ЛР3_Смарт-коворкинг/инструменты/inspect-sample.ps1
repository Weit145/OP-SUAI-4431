$ErrorActionPreference='Stop'
$api=New-Object -ComObject ProcessModeler70.SCAPI
$pu=$api.PersistenceUnits.Add('C:\Program Files (x86)\CA\AllFusion Process Modeler r7\Samples\Standard\Quill1.bp1','RDO=Yes')
$s=$api.Sessions.Add(); $s.Open($pu,0) | Out-Null
$segment=@($s.ModelObjects | Where-Object ClassName -eq PMArrowSegment)[0]
Get-Member -InputObject $segment.Properties
foreach($p in $segment.Properties){
 Write-Output ($p.ClassName+' Count='+$p.Count)
 $v=$p.GetType().InvokeMember('Value',[Reflection.BindingFlags]::GetProperty,$null,$p,@())
 Write-Output ('Type='+$v.GetType().FullName+' Value='+($v -join ','))
 if($v -is [array]){ Write-Output ('Rank='+$v.Rank); foreach($q in $v){Write-Output ('Element '+$q.GetType().FullName+' '+($q -join ','))}}
}
Write-Output ("PARENT "+$segment.Context.ClassName); foreach($o in @($segment.Context,$segment.Context.Context)){Write-Output ($o.ClassName+" "+$o.ObjectId); foreach($p in $o.Properties){Write-Output ($p.ClassName+"="+($p.GetType().InvokeMember("Value",[Reflection.BindingFlags]::GetProperty,$null,$p,@()) -join ","))}}
$p=$segment.Properties.Item("PMVertices"); $v=$p.GetType().InvokeMember("Value",[Reflection.BindingFlags]::GetProperty,$null,$p,@()); "DIMENSIONS $($v.GetLength(0)) x $($v.GetLength(1))"; for($i=0;$i -lt $v.GetLength(0);$i++){for($j=0;$j -lt $v.GetLength(1);$j++){"[$i,$j]=$($v[$i,$j])"}}; "ARROW PARENT "+$segment.Context.Context.ClassName
$pu.Save((Join-Path $PSScriptRoot "sample-export.xml")) | Write-Output
