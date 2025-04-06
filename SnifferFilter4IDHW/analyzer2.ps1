$port1 = new-Object System.IO.Ports.SerialPort COM10,115200
$port1.Open()
sleep 1

$hash = @{}
do {
#while ($true) {
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
                    if ($_.Length -lt 7) {
                        $stringToHash += $_ + "     ,";
                    } else {
                        $stringToHash += $_ + ",";
                    }
                    
                } else {
                    $stringToHash += $_ + ",";
                }
                $i++;
            }

            if ($canid -ne $null) {
                    if ($hash[$canid] -eq $null) {
                        $time = 0
                        $period = 0
                        #$hash.Add($canid, $s) #newcanid
                        $hash.Add($canid, $stringToHash) #newcanid
                    } else {
                        $time = $la[0]# -replace "<", ""
                        #$period = $time-($hash[$canid] -split ",")[0]
                        #$hash[$canid] = $s + "; " + $period +"ms" #update existing
                        $hash[$canid] = $stringToHash + "; " + $period +"ms" #update existing
                    }
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
     Start-Sleep -Milliseconds 100

     if ([System.Console]::KeyAvailable){
        $k=[System.Console]::ReadKey();
        if ($k.Key -eq "Q"){
            break;
        }
    }
#}
#} until ([System.Console]::KeyAvailable)
} until ($true)

#do {if ([System.Console]::KeyAvailable){$k=[System.Console]::ReadKey();if ($k.Key -eq "Q"){break;}else{write-host $k.Key}}} while ($true)

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

