let socket = new WebSocket("ws://192.168.137.30/ws");

let drinksData = {A:{name:"", drink:"", calibration: 650, value:0, refill: false}, B:{name:"", drink:"", calibration: 650, value:0, refill: false}}
let last = true;



function sendPercentage(name, cupSend, percentage){
  let payload = {NAME:name, CUP:cupSend, TYPE:"percentage", PERCENTAGE:percentage};
  socket.send(JSON.stringify(payload))
}

function sendRefillNotification(name, cupSend, drink){
  let payload = {NAME:name, CUP: cupSend, DRINK: drink, TYPE:"refill"};
  socket.send(JSON.stringify(payload));
}

socket.onopen = function(e) {
 console.log("hello world")
//  socket.send("hello world?")
};

socket.onmessage = function(event) {
 // alert(`[message] Data received from server: ${event.data}`);
 console.log(event)
 let message = event.data;
  try{
    message = JSON.parse(message)
    if(message.TYPE == 'cupUpdate'){
      drinksData[message.CUP].value = message.ANALOG;
        console.log(drinksData[message.CUP].value / drinksData[message.CUP].calibration)
       let percentage = Math.ceil((.0735 * Math.exp(7.27 * (drinksData[message.CUP].value / drinksData[message.CUP].calibration ))))
       if(percentage > 100){
         percentage = 100
       }
       else if(percentage < 4){
         percentage = 0;
         if(!drinksData[message.CUP].refill){
         sendRefillNotification(drinksData[message.CUP].name, message.CUP, drinksData[message.cup].drink)
         drinksData[message.CUP].refill = true;
         }
       }
      if(message.CUP == "A"){//0.0735e^7.27x

        setPercentage(percentage);
      }
      else{
        setPercentage2(percentage);


      }
 sendPercentage(drinksData[message.CUP].name, message.CUP, percentage)

    }
    else if(message.TYPE == 'calibrate'){
  drinksData[message.CUP].calibration = drinksData[message.CUP].value;
  drinksData[message.CUP].refill = drinksData[message.CUP].false;

    }
  }
  catch{
    console.log("prbly wasn't a json")
  }
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
    
    //if there is a drink, add it to the drinksData depending on if A or B is empty
    let cup = last ? "A" : "B";
    // if(drinksData[cup].drink == ""){
        drinksData[cup].name = name;
        drinksData[cup].drink = drink;
        drinksData[cup].refill = false;
        let id = "drink"+(last?"1" : "2")

        last = !last;
        console.log(id)
        document.getElementById(id).innerText = drink;
    // }

    let payload = {NAME:name, DRINK:drink, TYPE:"newPatron", CUP:cup};

    socket.send(JSON.stringify(payload))

}


document.getElementById("box1").onclick= function(){
  drinksData.A.calibration = drinksData.A.value;
}

document.getElementById("box2").onclick= function(){
  drinksData.B.calibration = drinksData.B.value;
}
// function test(name,drink)
// {
//     console.log(name,drink)
// }