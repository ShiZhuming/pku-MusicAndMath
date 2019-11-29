module.exports=function(data){
    return xdneiyc(data)+xdjianyc(data)
}
/*
0   1     2   3   4   5   6   7   8   9   10   11   12  
C         D       E   F       G       A        B    C   
1   16/15 9/8 6/5 5/4 4/3     3/2 8/5 5/3 16/9 15/8 2   

*/
const yincheng=[100,10,30,50,60,90,10]
function xdjianyc(data){
    if(data.length<2)return 50
    let sum=0
    for(let i=0;i<data.length-1;i++){
        let tempdata0=data[i]['pitch'],tempdata1=data[i+1]['pitch']
        if(tempdata0.length<1||tempdata1.length<1){
            sum+=50;continue
        }
        let tempsum=0
        for(let j=0;j<tempdata0.length;j++){
            for(let k=0;k<tempdata1.length;k++){
                let d=(tempdata1[k]-tempdata0[j])%12
                let D=(((tempdata1[k]-tempdata0[j])>=0)?(tempdata1[k]-tempdata0[j]):(-tempdata1[k]+tempdata0[j]))
                if(d<0)d+=12
                if(d>6){
                    tempsum+=yincheng[12-d]-D+6
                }else{
                    tempsum+=yincheng[d]-D+6
                }
            }
        }
        sum+=tempsum/(tempdata1.length*tempdata0.length)
    }
    return sum/(data.length-1)
}
function xdneiyc(data){
    if(data.length==0)return 50
    let sum=0
    for(let i=0;i<data.length;i++){
        let tempdata=data[i]['pitch']
        if(tempdata.length<2){
            sum+=50;continue
        }
        let tempsum=0
        for(let j=0;j<tempdata.length-1;j++){
            for(let k=j+1;k<tempdata.length;k++){
                let d=(tempdata[k]-tempdata[j])%12
                if(d<0)d+=12
                if(d==0){
                    tempsum+=50
                }
                else if(d>6){
                    tempsum+=yincheng[12-d]
                }
                else{
                    tempsum+=yincheng[d]
                }
            }
        }
        sum+=(tempsum*2/(tempdata.length*(tempdata.length-1)))
    }
    return sum/data.length
}