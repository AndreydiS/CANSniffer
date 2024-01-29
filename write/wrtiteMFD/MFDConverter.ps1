
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

#$Artist = "Manowar"
#$Album = "The Hell Of Steel (7567123"
#$Song = "Blow Your Speakers"

$Artist = "Helix Lite"
$Album = "v.6"
$Song = "Vol: 12 Sub: 92 DigIn: On"


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

$i=0
$startCode = "80,"
$consecutiveCode = 0xC0

write-host $startCode -NoNewline
$out -split "," | % {
    write-host ($_+",") -NoNewline
    $i++;
    if ($i -ge 7) {
        write-host
        write-host (('{0:X}' -f $consecutiveCode)+",") -NoNewline
        $consecutiveCode++
        $i=0
    }
}
write-host
