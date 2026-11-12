param([string]$DocumentPath)
$ErrorActionPreference='Stop'
$word=New-Object -ComObject Word.Application
$word.Visible=$false
$word.DisplayAlerts=0
$word.AutomationSecurity=3
$doc=$null
try{
 $doc=$word.Documents.Open($DocumentPath,$false,$false)
 $doc.Fields.Update() | Out-Null
 foreach($toc in $doc.TablesOfContents){$toc.Update()}
 $doc.Repaginate()
 $doc.Save()
 $pdfPath=[IO.Path]::ChangeExtension($DocumentPath,'.pdf')
 $doc.ExportAsFixedFormat($pdfPath,17)
 Write-Output ('Pages: '+$doc.ComputeStatistics(2)+'; images: '+$doc.InlineShapes.Count+'; PDF: '+$pdfPath)
}finally{
 if($doc){$doc.Close(0)}
 if($word.Documents.Count -eq 0){$word.Quit()}
 [Runtime.InteropServices.Marshal]::ReleaseComObject($word) | Out-Null
}
