param([string]$ModelPath)
$ErrorActionPreference='Stop'
$api=New-Object -ComObject ProcessModeler70.SCAPI
$pu=$api.PersistenceUnits.Add($ModelPath,'RDO=Yes')
$s=$api.Sessions.Add();$s.Open($pu,0) | Out-Null
$activities=@($s.ModelObjects | Where-Object ClassName -eq PMActivity)
$arrows=@($s.ModelObjects | Where-Object ClassName -eq PMArrow)
$segments=@($s.ModelObjects | Where-Object ClassName -eq PMArrowSegment)
if($activities.Count -ne 1 -or $arrows.Count -ne 14 -or $segments.Count -ne 14){throw 'Unexpected model object counts'}
$s.Close() | Out-Null
Write-Output 'Native BP1 reopened: 1 activity, 14 arrows, 14 segments.'
