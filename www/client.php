<?php
	$sock=socket_create(AF_INET,SOCK_DGRAM,0);
        if(!$sock)
	{
                echo "socket create failure";
        }

	$server_ip="127.0.0.1";
	$port = 8888;

	// reset data
        $buf = "";
        $msg = "";

	$buf = $_POST["buf"];
        if($buf=="")
	{
                $buf="hello, how are you!\n";
	}

	if(!socket_sendto($sock,$buf,strlen($buf),0,$server_ip,$port))
	{
		echo "send error\n";
		socket_close($sock);
		exit();
	}

	// ouput the sending message
	echo "send: ";
        echo $buf."<br>";


	$msg = "";
	if(!socket_recvfrom($sock, $msg, 256, 0, $server_ip, $port))
	{
		echo "recvieve error!";
		socket_close($sock);
		exit();
	}

	// output the receive message
	echo "receive: ";
	echo $msg."<br>";
	
	socket_close($sock);
?>

<form action="client.php" method="post">
<input type=text name=buf>
<input type=submit value="submit">
</form>
