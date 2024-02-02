
#80	41	4c	55	12	42	6c	6f		41	4c	55	12	B	l	o	lenght of song
#c0	77	20	59	6f	75	72	20		w	_	Y	o	u	r	_	
#c1	53	70	65	61	6b	65	72		S	p	e	a	k	e	r	
#c2	73	48	0	0	7	4d	61		s	48	0	0	7	M	a	lenght of Artist
#c3	6e	6f	77	61	72	49	1a		n	o	w	a	r	49	1a	lenght of album
#c4	54	68	65	20	48	65	6c		T	h	e	_	H	e	l	
#c5	6c	20	4f	66	20	53	74		l	_	O	f	_	S	t	
#c6	65	65	6c	20	28	37	35		e	e	l	_	(	37	35	
#c7	36	37	ee	80	89	4a	0		36	37	ee	80	89	4a	0	Format of the message?
#c8	0	1	0	0	0				0	1	0	0	0			

#$Song   //bottom(middle)
#$Artist //top
#$Album  //middle

#$Artist = "Tic Tac Toe"
#$Album = "Tic Tac Toe"
#$Song = "Verpiss' Dich"

$comPort = "COM4"

$Artist = "Helix Lite"
$Album = "v.6"
$Song = "Vol: 12 Sub: 92 DigIn: On "


$Artist = "manowarabcdef123" # max Manowarabcdef12... manowarabcdef12... manowarabcdef123 
$Album = "the hell of steel (71" #max The Hell Of Steel (...    the hell of steel (7...    
$Song = "B`nYT2" #max Blow Your Speakers Blow ... # 2 lines only  `n may be used for new line
$CANID = "17333111"
#$CANID = "5bf"

$canIDSplit = ""
if ($CANID.length%2 -eq 1) {$CANID="0"+$CANID}
for ($i=$CANID.length-1;$i -ge 0 ;$i--) {$canIDSplit=$CANID[$i]+$canIDSplit; if ($i%2 -eq 0) {$canIDSplit=","+$canIDSplit}};
$serialCMDprefix="61,74,20,"+[math]::ceiling($CANID.length/2)+$canIDSplit+","
$serialCMDprefix

$out = "<LM>,4c,55,"
$out +=('{0:X}' -f $Song.Length) + ","

$Song.ToCharArray() | % {$hex= ([Convert]::ToString(([byte][char]$_),16)); $out += $hex+"," }
$out += "48,00,00,"+ ('{0:X}' -f $Artist.Length) + ","
$Artist.ToCharArray() | % {$hex= ([Convert]::ToString(([byte][char]$_),16)); $out += $hex+"," }
$out += "49,"+ ('{0:X}' -f $Album.Length) + ","
$Album.ToCharArray() | % {$hex= ([Convert]::ToString(([byte][char]$_),16)); $out += $hex+"," }
$out += "4a,00,00,01,00,00,00"

$MessageLenght = ($out -split "," | measure).count -3

$out=$out.replace("<LM>", ('{0:X}' -f $MessageLenght))

$startCode = 0x80
$consecutiveCode = 0xBF#0xC0
$toSerial=""
$i=0

$out -split "," | % {
    if ($i -eq 0) {
        if ($consecutiveCode -eq 0xBF) {
            $prefixByte = $startCode
        } else {
            $prefixByte = $consecutiveCode
        }
        write-host (('{0:X}' -f $prefixByte)+",") -NoNewline
        $toSerial += ('{0:X}' -f $prefixByte)+","
        $consecutiveCode++;
    }
    $toSerial += $_+","
    write-host ($_+",") -NoNewline
    if ($i -ge 6) {
        write-host
        $i=0
    } else {
        $i++
    }
}

$bytesToSendInOneTransmission = 33

$b =[byte]1..($bytesToSendInOneTransmission+1)
for ($i=0;$i -le $bytesToSendInOneTransmission;$i++) {
    $b[$i]=[byte]0x00
}

$port1 = new-Object System.IO.Ports.SerialPort $comPort,9600
$port1.Open()
sleep 1
$port1.ReadExisting()


$byteCount = 0
($serialCMDprefix+$toSerial) -split "," | % {
    $byte = $_
    if ($byte -ne "") {
        $b[$byteCount] = [byte][System.Convert]::ToInt64($byte, 16)
        if ($byteCount -ge $bytesToSendInOneTransmission) {
                $port1.Write($b,0,$bytesToSendInOneTransmission+1)
                $byteCount = 0
                sleep .6

            } else { $byteCount++ }
    }
}
if ($byteCount -gt 0) { #something left to sent
    $port1.Write($b,0,$byteCount)
}

sleep 2
$port1.ReadExisting()

$port1.Close()
