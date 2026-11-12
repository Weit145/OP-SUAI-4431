param([string]$FilePath)
$ErrorActionPreference='Stop'
Add-Type -AssemblyName System.Drawing
Add-Type @'
using System;using System.Runtime.InteropServices;
public class PictureExport {
[DllImport("user32.dll")]public static extern bool PostMessage(IntPtr h,uint m,IntPtr w,IntPtr l);
[DllImport("user32.dll")]public static extern bool OpenClipboard(IntPtr h);
[DllImport("user32.dll")]public static extern IntPtr GetClipboardData(uint f);
[DllImport("user32.dll")]public static extern bool CloseClipboard();
[DllImport("gdi32.dll",CharSet=CharSet.Unicode)]public static extern IntPtr CopyEnhMetaFile(IntPtr h,string f);
}
'@
[PictureExport]::PostMessage([IntPtr]459590,273,[IntPtr]25285,[IntPtr]0)|Out-Null
Start-Sleep -Milliseconds 400
if(![PictureExport]::OpenClipboard([IntPtr]0)){throw 'Clipboard unavailable'}
try{$handle=[PictureExport]::GetClipboardData(14);if($handle -eq [IntPtr]0){throw 'No diagram in clipboard'};$emfPath=[IO.Path]::ChangeExtension($FilePath,'.emf');$copy=[PictureExport]::CopyEnhMetaFile($handle,$emfPath); Write-Output ('EMF handle='+$handle+' copy='+$copy)}finally{[PictureExport]::CloseClipboard()|Out-Null}
$meta=New-Object System.Drawing.Imaging.Metafile($emfPath)
$scale=2
$bitmap=New-Object System.Drawing.Bitmap(($meta.Width*$scale),($meta.Height*$scale))
$graphics=[System.Drawing.Graphics]::FromImage($bitmap)
$graphics.Clear([System.Drawing.Color]::White)
$graphics.DrawImage($meta,0,0,$bitmap.Width,$bitmap.Height)
$bitmap.Save($FilePath,[System.Drawing.Imaging.ImageFormat]::Png)
$graphics.Dispose();$bitmap.Dispose();$meta.Dispose()
Write-Output $FilePath

