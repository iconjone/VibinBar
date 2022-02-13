let socket = new WebSocket("ws://192.168.179.51/ws");

let drinksData = {A:{name:"", drink:""}, B:{name:"", drink:""}}

socket.onopen = function(e) {
 console.log("hello world")
//  socket.send("hello world?")
};

socket.onmessage = function(event) {
 // alert(`[message] Data received from server: ${event.data}`);
 console.log(event)
 let message = event.data;
 console.log(message)
};

socket.onclose = function(event) {
  if (event.wasClean) {
    alert(`[close] Connection closed cleanly, code=${event.code} reason=${event.reason}`);
  } else {
    // e.g. server process killed or network down
    // event.code is usually 1006 in this case
    alert('[close] Connection died');
  }
};

socket.onerror = function(error) {
  alert(`[error] ${error.message}`);


};
function newPatron(name, drink)
{
    let payload = {NAME:name, DRINK:drink, TYPE:"newPatron"}
    
    
    socket.send(JSON.stringify(payload))

}
// function test(name,drink)
// {
//     console.log(name,drink)
// }