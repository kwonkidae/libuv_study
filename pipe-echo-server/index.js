var net = require('net');

var stream = net.connect('/tmp/echo.sock');
setTimeout(() => {
  for(let i = 0; i < 1000; i++) {
    await 
    stream.write(i + 'hello\n');

  }
}, 1000);

stream.on('data', function(chunk){
  console.log('recv:' + chunk);
})
