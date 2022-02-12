document.addEventListener('keydown', registerKeyStrokes)

let buffer = []
function  registerKeyStrokes(event) {
          // const charList = 'abcdefghijklmnopqrstuvwxyz0123456789';
          const key = event.key.toLowerCase()
          // // we are only interested in alphanumeric keys
          // if (charList.indexOf(key) === -1) return;
          buffer.push(key)
          // console.log(buffer);
          let minusIndex = -1
          let periodIndex = -1
          if (key === 'enter') {
            for (let i = 0; i < buffer.length; i++) {
              if (buffer[i] === '-') {
                minusIndex = i
              }
              if (buffer[i] === '.') {
                periodIndex = i
              }
            }
            if (minusIndex === -1 && periodIndex === -1) {
              buffer = []
            } else {
              let str = ''
              for (let i = minusIndex; i < buffer.length - 1; i++) {
                if (buffer[i] !== 'shift') {
                  str += buffer[i]
                }
              }
              //console.log(str)
              let date = ""
              let nameWork = ""
              let username = []
             let DataArr =  str.split("^")
             console.log(DataArr)
             nameWork = DataArr[1];
             nameWork = (nameWork.split("$"))
             username[0] = capitalizeFirstLetter(nameWork[0])
             username[1] = capitalizeFirstLetter(nameWork[1])

             document.getElementById("nameMessage").innerText = "Hey " + username[1] + " "+  username[0] +"!"
             console.log(DataArr[3])
             let DOB = DataArr[3]
             DOB =  DOB.split("?")
             DOB = DOB[1]
             DOB =JSON.stringify(DOB)
             console.log(typeof(DOB))
             DOB = DOB.slice(21)
             let year = DOB.substring(0,4)
             console.log(year)
             let month = DOB.substring(4,6)
             if(month<10){month = convertDate(month)}
             console.log(month)
             let day = DOB.substring(6,8)
             if(day<10){day = convertDate(day)}
             console.log(day)
             let legalDate = [2001,2,12]
             isLegal(year,month,day)
            

          }
        }
    }
        function capitalizeFirstLetter(string) {
            return string.charAt(0).toUpperCase() + string.slice(1);
          }  
        function isLegal(year,month,day){
           days = (year*365)+
        }
        function convertDate(date){
            date = date.slice(1)
            return date
        }

    



