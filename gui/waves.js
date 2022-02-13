var cnt=document.getElementById("count"); 
var water=document.getElementById("water");
var cnt2=document.getElementById("count2"); 
var water2=document.getElementById("water2");
var percent=cnt.innerText;
var percent2=cnt2.innerText;
var interval;
var interval2;




function setPercentage(percentage){
    clearInterval(interval);
    console.log("per1", percentage)

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

function setPercentage2(percentage){
    clearInterval(interval2);
    console.log("per2", percentage)
    if(percentage != percent2){
    console.log(percentage)
    if(percentage > percent2){
        console.log("this")
        interval2=setInterval(function(){ 
            percent2++; 
            cnt2.innerHTML = percent2; 
            water2.style.transform='translate(0'+','+(100-percent2)+'%)';
            if(percent2==percentage){
              clearInterval(interval2);
            }
          },60);
    }
    else if(percentage < percent2){
        console.log("that")
        interval2=setInterval(function(){ 
            percent2--; 
            cnt2.innerHTML = percent2; 
            water2.style.transform='translate(0'+','+(100-percent2)+'%)';
            if(percent2==percentage){
              clearInterval(interval2);
            }
          },60);
    }
}
}


// setPercentage(60)
// setPercentage2(80)
// setTimeout(()=>{
//     setPercentage(40)
//     setPercentage2(10)
// }, 30 * 1000)

// setTimeout(()=>{
//     setPercentage(80)
//     setPercentage2(50)
// }, 60 * 1000)

// setTimeout(()=>{
//     setPercentage(20)
//     setPercentage2(100)
// }, 90 * 1000)
// // setPercentage(20)