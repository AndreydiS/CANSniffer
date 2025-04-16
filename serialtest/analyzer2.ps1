$port1 = new-Object System.IO.Ports.SerialPort COM10,115200
$port1.Open()
sleep 1

$boolRecord=$false;
$arrRecord = @();

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
                    $stringToHash += $_ + (addSpaces(7-$_.Length)) + ",";
                    # if ($_.Length -lt 7) {
                    #     $stringToHash += $_ + "     ,";
                    # } else {
                    #     $stringToHash += $_ + ",";
                    # }                    
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
                        stringToHash = stringToHash + (addSpaces(42-$stringToHash.Length))
                        $hash[$canid] = $stringToHash + "; " + (addSpaces(6-$period.ToString().Length)) + $period +"ms" #update existing
                    }
            }
            $arrRecord += $stringToHash;
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
     Start-Sleep -Milliseconds 25;#50-100

     if ([System.Console]::KeyAvailable){
        $k=[System.Console]::ReadKey();
        switch ($k.Key) {
            "q" {$exitDo = $true;break;}
            "r" {$boolRecord = $true;break;}
        }
        if ($exitDo) {break;}
    }
} until ($false)

if ($boolRecord) {
    write-host "Recorded Packets=========================================="
    foreach($e in $arrRecord) {
        write-host $e;
    }
}


$port1.Close()


function addSpaces($intNumberOfSpaces) {
    if ($intNumberOfSpaces -lt 0) {
        return "";
    } else {
        return (" "*$intNumberOfSpaces); 
    }
}

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

