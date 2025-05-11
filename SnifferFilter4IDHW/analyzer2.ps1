$ReadMillisecondsDelay = 20;

$port1 = new-Object System.IO.Ports.SerialPort COM10,115200
#$port1 = new-Object System.IO.Ports.SerialPort COM10,57600
$port1.Open()
sleep 1

$b = 0x73,0x20,0x00,0x00,0x00,0x00

$port1.Write($b,0,6)

function addSpaces($intNumberOfSpaces) {
    if ($intNumberOfSpaces -lt 0) {
        return "";
    } else {
        return (" "*$intNumberOfSpaces); 
    }
}


$boolRecord=$false;
$arrRecord = @();

$hashKnownCanids = @{}
$hashKnownCanids.add("fd","esp and speed")
$hashKnownCanids.add("107","MO. speed gear oilPres rpm boost")
$hashKnownCanids.add("116","ESP. wheel rotation")
$hashKnownCanids.add("30b","Kombi. warning lights")
$hashKnownCanids.add("366","turn signal bulbs")
$hashKnownCanids.add("394","Gearbox. gears")
$hashKnownCanids.add("3be","MO.")
$hashKnownCanids.add("3c0","Klemmen status.")
$hashKnownCanids.add("3c7","MO. Oil Level")
$hashKnownCanids.add("3d5","Lights. Request")
$hashKnownCanids.add("3da","Park Assist or camera. Speed steer angle button")
$hashKnownCanids.add("3db","GW. klima, door open, lights, outside temp")
$hashKnownCanids.add("3dc","GW. Gear Selector")
$hashKnownCanids.add("520","Airbag. Seat belt and usage")
$hashKnownCanids.add("5f0","Dim cluster. fotosensor") 
$hashKnownCanids.add("5bf","wheel button")
$hashKnownCanids.add("640","MO. air oil cooliant temp")
$hashKnownCanids.add("641","MO. code")
$hashKnownCanids.add("643","Unit. time volume lang")
$hashKnownCanids.add("647","MO. cooliant temp starts co2")
$hashKnownCanids.add("658","Lights front")
$hashKnownCanids.add("65a","Lights on cluster?")
$hashKnownCanids.add("65d","ESP. teeth wheel circ.")
$hashKnownCanids.add("663","voltage")
$hashKnownCanids.add("668","climat buttons to HU")
$hashKnownCanids.add("66e","Klima")
$hashKnownCanids.add("670","MO. number of cyl speed warning")
$hashKnownCanids.add("6af","from HU to camera control bri, contrast, etc")
$hashKnownCanids.add("6b2","Diag. milage time date")
$hashKnownCanids.add("6b4","VIN")
$hashKnownCanids.add("6b7","Kombi. milage servicelife fuel in tank outside temp")
$hashKnownCanids.add("6b8","Kombi. tire circ. fuel levels")
$hashKnownCanids.add("17330110","Climat to HU display")
$hashKnownCanids.add("17331110","Time on HU")
$hashKnownCanids.add("17333110","HU volume")
$hashKnownCanids.add("17333111","mess from HU to MFD")

$hashDontSendCanids = @{}
$hashDontSendCanids.add("6b4","VIN")

$hash = @{}
$exitDo = $false;
do {
    $rows = [console]::WindowHeight - 2 # account for headers
    $columns = [console]::WindowWidth

    $c=$port1.ReadExisting()

    $c -split "`r`n" | % {
        if ($_ -like "<*>") {
            $s = ($_ -replace "<", "") -replace ">", ""
            $la = $s -split ","
            $canid = $la[1] #[Convert]::ToInt32($a, 16)

            $stringToHash = ""
            $i=0;
            ($s -split ",") | % {
                if ($i -eq 1) {
                    $stringToHash += $_ + (addSpaces(8-$_.Length)) + ",";
                } else {
                    $stringToHash += $_ + ",";
                }
                $i++;
            }

            if ($canid -ne $null) {
                    if ($hash[$canid] -eq $null) {
                        $time = 0
                        $period = 0
                        $hash.Add($canid, $stringToHash) #newcanid
                    } else {
                        $time = $la[0]# -replace "<", ""
                        $period = $time-($hash[$canid] -split ",")[0]
                        $stringToHash = $stringToHash + (addSpaces(42-$stringToHash.Length))
                        $hash[$canid] = $stringToHash + "; " + (addSpaces(6-$period.ToString().Length)) + $period +"ms " + $hashKnownCanids[$canid] #update existing
                    }
            }
            if ($boolRecord) {
                $arrRecord += $stringToHash;
            }
        }
    }
    $newText = (
        $hash.GetEnumerator() | Sort-Object -Property Key |
               Select-Object -first $rows |
               Format-Table |
               Out-String -Stream |                 # get output as individual lines
               Where-Object {$_} |                  # get rid of trailing lines
               ForEach-Object PadRight $columns ' ' # overwrite gack from last time
    ) -join "`n"

     [console]::SetCursorPosition(0,0)
     [console]::Write($newText)
     [console]::SetCursorPosition(0,0)
     Start-Sleep -Milliseconds $ReadMillisecondsDelay;#50-100

     if ([System.Console]::KeyAvailable){
        $k=[System.Console]::ReadKey();
        switch ($k.Key) {
            "q" {$exitDo = $true;break;}
            "r" {$boolRecord = $true;break;}
        }
        if ($exitDo) {break;}
    }
} until ($false)

$fileout = "recorded.txt"
if ($boolRecord) {
    cls
    write-host "Recorded Packets=========================================="
    $time=0;
    "" | out-file $fileout
    foreach($s in $arrRecord) {
        $s | out-file $fileout -Append
    }
}


$port1.Close()

# for() {
#     $rows = [console]::WindowHeight - 2 # account for headers
#     $columns = [console]::WindowWidth
  
#     $newText = (
#       Get-Process |
#       Sort-Object CPU -Descending |
#       Select-Object -first $rows |
#       Format-Table |
#       Out-String -Stream |                 # get output as individual lines
#       Where-Object {$_} |                  # get rid of trailing lines
#       ForEach-Object PadRight $columns ' ' # overwrite gack from last time
#     ) -join "`n"
  
#     [console]::SetCursorPosition(0,0)
#     [console]::Write($newText)
#     [console]::SetCursorPosition(0,0)
#     Start-Sleep -Milliseconds 50
#   }

