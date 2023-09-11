const net = require('net');

var t = 10;
var string = '{"data": 69}';
var obj = {'data': 69};
var json = JSON.parse(string);

const server = net.createServer(function(socket){
	console.log('A client connected');
	socket.on('data', function(data){
		socket.write(JSON.stringify(obj));
	});

	socket.on('end', function(){
		obj['data'] += 1
		console.log('Client disconnected');
	});
});

const port = 3000;
server.listen(port, "127.0.0.1", function(){
	console.log(`Server is listening on port ${port}`);
});

