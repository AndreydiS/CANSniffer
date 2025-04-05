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
            $la = $_ -split ","
            $canid = $la[2] #[Convert]::ToInt32($a, 16)
            if ($canid -ne $null) {
                    if ($hash[$canid] -eq $null) {
                        $hash.Add($canid, $_)
                    } else {
                        $hash[$canid] = $_
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
#}
} until ([System.Console]::KeyAvailable)


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

