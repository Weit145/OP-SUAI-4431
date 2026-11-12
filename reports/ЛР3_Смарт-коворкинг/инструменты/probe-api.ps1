$ErrorActionPreference='Stop'
$api=New-Object -ComObject ProcessModeler70.SCAPI
$bag=New-Object -ComObject ProcessModeler70.SCAPI.PropertyBag
$bag.Add('Name','Smart Coworking') | Out-Null
$bag.Add('ModelType',0) | Out-Null
$pu=$api.PersistenceUnits.Create($bag)
Write-Output ('PU: '+$pu.Name)
$session=$api.Sessions.Add()
$session.Open($pu,0) | Write-Output
Get-Member -InputObject $session.ModelObjects.Root
foreach($obj in $session.ModelObjects){
 Write-Output ('OBJECT '+$obj.ClassName+' '+$obj.ObjectId)
 if($obj.ClassName -notmatch 'PMActivity|PMDiagram|PMBox'){continue}
 foreach($prop in $obj.Properties){try{Write-Output ('  '+$prop.ClassName+' = '+($prop.GetType().InvokeMember('Value',[System.Reflection.BindingFlags]::GetProperty,$null,$prop,@()) -join ','))}catch{Write-Output $_}}
}
$pu.Save((Join-Path $PSScriptRoot 'test-idef0.bp1')) | Write-Output

