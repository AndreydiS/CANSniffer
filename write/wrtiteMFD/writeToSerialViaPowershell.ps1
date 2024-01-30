[byte[]] $b = 0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37

$port1 = new-Object System.IO.Ports.SerialPort COM8,115200
$port1.Open()

#$port1.Write([byte]0x61,0,1)
#sleep 1
#$port1.ReadExisting()



$port1.Write($b,0,8)
sleep 1
$port1.ReadExisting()

$port1.Close()
