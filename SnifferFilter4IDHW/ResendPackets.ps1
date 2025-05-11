$fileout = "recorded.txt"

$arrRecord = Get-Content $fileout

$port1 = new-Object System.IO.Ports.SerialPort COM10,115200
#$port1 = new-Object System.IO.Ports.SerialPort COM3,115200
#$port1 = new-Object System.IO.Ports.SerialPort COM10,57600
$port1.Open()

cls
$time=0;
#$prevticks=(get-date).ticks/10000;
foreach($s in $arrRecord) {
    if ($s.trim().Length -gt 1) {
        #$ticks=(get-date).ticks/10000;write-host "   ms1 " ($ticks -$prevticks);$prevticks = $ticks
        $la = $s -split ","
        #$ticks=(get-date).ticks/10000;write-host "   ms2 " ($ticks -$prevticks);$prevticks = $ticks
        #$ticks=(get-date).ticks/10000;write-host "   ms3 " ($ticks -$prevticks);$prevticks = $ticks
        $canid=[System.Convert]::ToInt32($la[1].trim(), 16)
        #$ticks=(get-date).ticks/10000;write-host "   ms4 " ($ticks -$prevticks);$prevticks = $ticks

        #if ($canid -gt 0x0) { #all
        #if ($canid -le 0x7ff) { #std only
        #if ($canid -gt 0x7ff) { #ext only
        #if (($canid -gt 0x7ff) -and ($canid -le 0x17330B10)) {
        #if (($canid -gt 0x17330B10) -and ($canid -le 0x17330910)) {
        #if (($canid -gt 0x17330910)) {
        #if (($canid -gt 0x17330910) -and ($canid -le 0x17331310)) {
        #if (($canid -gt 0x17330910) -and ($canid -le 0x17331110)) {
        #if (($canid -gt 0x17330f10) -and ($canid -le 0x17331110)) {

        #if (($canid -eq 0x17330F10)) { #outside temp to HU
        if (($canid -eq 0x17333110)) { #messages from HU to MFD

            #if (($canid -gt 0x17331110) -and ($canid -le 0x17331310)) {

            
            #if (($canid -gt 0x17331310)) {

            #if ($canid -eq 0x5bf) {
            #if ($canid -eq 0x107) {
            #if ($canid -eq 0x366) {
            #if (($canid -gt 0x17330000) -and ($canid -le 0x17330B10)) {
         #if (($canid -gt 0x17330010) -and ($canid -le 0x17330910)) {
            #if (($canid -gt 0x17330010) -and ($canid -le 0x17330510)) {

            #if (($canid -gt 0x17330910) -and ($canid -le 0x17330B10)) {

            #0x1FFFFFFF
            
            write-host $s;
            #     #Serial.println("CAN ok. CMD send: 74 20 <canid_4b> <payloadlen_1b> <payload>");
            #[byte[]] $b2 = 0x74,0x20,0x00,0x00,0x05,0xbf,0x04,0x12,0x00,0x01,0x23
            if ($time -ne 0) {
                #write-host (Get-Date).ToString("yyyy-MM-dd HH:mm:ss.fff") -ForegroundColor green
                $delayBySent= ((get-date).ticks/10000) - $timeSent
                $delayByRecord = $la[0]-$time;
                if ($delayBySent -lt $delayByRecord) {
                    Start-Sleep -Milliseconds ($delayByRecord-$delayBySent)
                    write-host "waiting ms:" $delayByRecord $delayBySent ($delayByRecord-$delayBySent) -ForegroundColor Yellow
                } else {
                    write-host "no wait. delay by sent is greater" $delayByRecord $delayBySent ($delayByRecord-$delayBySent) -ForegroundColor Yellow
                }
            }
    
            [byte[]] $b = 0x74,0x20,(($canid -shr 24) -band 0xFF),(($canid -shr 16) -band 0xFF),(($canid -shr 8) -band 0xFF),($canid -band 0xFF),($la.Length-3), 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
            for($i=2;$i -le ($la.Length-2); $i++) {
                $b[$i+5] = [System.Convert]::ToInt32($la[$i].trim(), 16)
            }

            #$b | % {write-host ("{0:X}" -f $_) " " -NoNewline}
            $port1.Write($b,0,7+($la.Length-3))
            $time = $la[0];
            $timeSent=(get-date).ticks/10000
            #write-host (Get-Date).ToString("yyyy-MM-dd HH:mm:ss.fff") -ForegroundColor green
            #start-sleep 1;
            $port1.ReadExisting();
            
        }
        
        
        if ([System.Console]::KeyAvailable){
            $k=[System.Console]::ReadKey();
            switch ($k.Key) {
                "q" {$exitDo = $true;break;}
            }
            if ($exitDo) {break;}
        }
    }
}

$port1.Close()
