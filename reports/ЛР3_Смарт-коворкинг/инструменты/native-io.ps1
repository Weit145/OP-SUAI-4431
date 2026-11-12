param([ValidateSet('Open','SaveAs')][string]$Action,[string]$FilePath,[int]$Format=3)
$ErrorActionPreference='Stop'
Add-Type -AssemblyName UIAutomationClient
Add-Type @'
using System; using System.Text; using System.Runtime.InteropServices;
public class NativeIO {
[DllImport("user32.dll",CharSet=CharSet.Unicode)]public static extern IntPtr SendMessage(IntPtr h,uint m,IntPtr w,string l);
[DllImport("user32.dll")]public static extern IntPtr SendMessage(IntPtr h,uint m,IntPtr w,IntPtr l);
[DllImport("user32.dll",CharSet=CharSet.Unicode)]public static extern int GetClassName(IntPtr h,StringBuilder s,int n);
[DllImport("user32.dll")]public static extern IntPtr GetParent(IntPtr h);
[DllImport("user32.dll")]public static extern bool PostMessage(IntPtr h,uint m,IntPtr w,IntPtr l);
}
'@
$process=Get-Process AFPM70 | Select-Object -First 1
$uiWindow=[System.Windows.Automation.AutomationElement]::RootElement.FindAll([System.Windows.Automation.TreeScope]::Children,[System.Windows.Automation.Condition]::TrueCondition) | Where-Object { $_.Current.ProcessId -eq $process.Id -and $_.Current.Name -like 'AllFusion*' } | Select-Object -First 1
$main=[IntPtr]$uiWindow.Current.NativeWindowHandle
$command=if($Action -eq 'Open'){25201}else{25204}
[NativeIO]::PostMessage($main,273,[IntPtr]$command,[IntPtr]0) | Out-Null
Start-Sleep -Milliseconds 700
$app=[System.Windows.Automation.AutomationElement]::FromHandle($main)
$dialog=$null; $edit=[IntPtr]0; $combo=[IntPtr]0
foreach($el in $app.FindAll([System.Windows.Automation.TreeScope]::Descendants,[System.Windows.Automation.Condition]::TrueCondition)){
 $c=$el.Current
 if($c.ControlType -eq [System.Windows.Automation.ControlType]::Window -and $c.AutomationId -eq '' -and $c.NativeWindowHandle -ne 0 -and [NativeIO]::GetParent([IntPtr]$c.NativeWindowHandle) -eq $main){$dialog=$el}
 if($c.AutomationId -eq '1148'){
  $sb=New-Object Text.StringBuilder 128
  [NativeIO]::GetClassName([IntPtr]$c.NativeWindowHandle,$sb,128) | Out-Null
  if($sb.ToString() -eq 'Edit'){$edit=[IntPtr]$c.NativeWindowHandle}
 }
 if($c.AutomationId -eq '1136'){$combo=[IntPtr]$c.NativeWindowHandle}
}
if(!$dialog -or $edit -eq [IntPtr]0){throw 'File dialog not found'}
if($Action -eq 'SaveAs'){
 [NativeIO]::SendMessage($combo,334,[IntPtr]$Format,[IntPtr]0) | Out-Null
 [NativeIO]::SendMessage([IntPtr]$dialog.Current.NativeWindowHandle,273,[IntPtr](1136+65536),$combo) | Out-Null
}
[NativeIO]::SendMessage($edit,12,[IntPtr]0,$FilePath.Replace('/','\')) | Out-Null
[NativeIO]::PostMessage([IntPtr]$dialog.Current.NativeWindowHandle,273,[IntPtr]1,[IntPtr]0) | Out-Null
Start-Sleep -Milliseconds 1000
Write-Output ((Get-Process AFPM70 | Select-Object -First 1).MainWindowTitle)


