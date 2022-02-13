var cnt=document.getElementById("count"); 
var water=document.getElementById("water");
var percent=cnt.innerText;
var interval;



function setPercentage(percentage){
    console.log(percentage)
    if(percentage > percent){
        interval=setInterval(function(){ 
            percent++; 
            cnt.innerHTML = percent; 
            water.style.transform='translate(0'+','+(100-percent)+'%)';
            if(percent==percentage){
              clearInterval(interval);
            }
          },60);
    }
    else if(percentage < percent){
        interval=setInterval(function(){ 
            percent--; 
            cnt.innerHTML = percent; 
            water.style.transform='translate(0'+','+(100-percent)+'%)';
            if(percent==percentage){
              clearInterval(interval);
            }
          },60);
    }
}


setPercentage(60)
setTimeout(()=>{
    setPercentage(40)
}, 30 * 1000)

setTimeout(()=>{
    setPercentage(80)
}, 60 * 1000)

setTimeout(()=>{
    setPercentage(20)
}, 90 * 1000)
// setPercentage(20)