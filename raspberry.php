<?php
	function ledOn()
	{
		$retour = file_get_contents('http://192.168.43.59/LEDON');
		return $retour;
	}

	function ledOff()
	{
		$retour = file_get_contents('http://192.168.43.59/LEDON');
		return $retour;
	}

	if(isset($_GET["led"]))
	{
		if($_GET["led"]=="on"){$content = json_decode(ledOn());}
		if($_GET["led"]=="off"){$content = json_decode(ledOff());}
	}else{
		$content = json_decode(ledOff());
	}

	echo('<a href="?led=on">LED ON</a><br>');
	echo('<a href="?led=off">LED OFF</a><br>');

	$time = date("Y-m-d H:i:s",time());
	$poto = $content->potentiometer;
	$photo = $content->photoresistor;
	print("potentiometre : ".$poto->value);
	print("<br>photoresistor : ".$photo->value);
	echo("<br>".$time);
?>