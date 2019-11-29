const eva = require('./evaluate')
const fs = require("fs");
const exec = require('child_process').exec;
const center = 69
var reads = []
for (let i = 0; i < 10; i++) {
    reads[i] = new Promise((resolve, reject) => {
        fs.readFile(String(i) + '.txt', (err, data) => {
            if (err) {
                reject(err)
            } else {
                data = String(data).split('\n').slice(0, -1)
                for (let j = 0; j < data.length; j++) {
                    data[j] = data[j].split(' ')
                    for (let k = 0; k < data[j].length; k++) {
                        data[j][k] = Number(data[j][k])
                    }
                    data[j] = {
                        time: data[j][0],
                        pitch: data[j].slice(1)
                    }
                }
                resolve(data)
            }
        })
    })
}
Promise.all(reads).then((data) => {
    for (let i = 0; i < 100000; i++) {
        genetic(data)
    }
    let max = eva(data[0]), pmax = 0
    for (let i = 0; i < data.length; i++) {
        let tempeva = eva(data[i])
        if (tempeva > max) {
            max = tempeva; pmax = i
        }
    }
    data = data[pmax]
    for (let i = 0; i < data.length; i++) {
        data[i] = data[i]['time'] + ' ' + data[i]['pitch'].join(' ')
    }
    data = data.join('\n') + '\n'
    fs.writeFile('in.txt', data, () => {
        exec('python tomidi.py;musescore out.mid', () => { return 0 })
    })
})
function genetic(data) {
    //繁殖
    {
        //选最好两个片段
        let max0 = eva(data[0]), max1 = max0, pmax0 = 0, pmax1 = 0;
        for (let i = 0; i < data.length; i++) {
            let tempeva = eva(data[i])
            if (tempeva > max0) {
                max1 = max0; pmax1 = pmax0;
                max0 = tempeva; pmax0 = i;
            }
            else if (tempeva > max1) {
                max1 = tempeva; pmax1 = i;
            }
        }
        //繁殖两个理想后代 即简单复制
        let child0 = data[pmax0], child1 = data[pmax1]
        //得到这两个理想后代的小段长度分布
        let len0 = [[], [], [], [], [], []], len1 = [[], [], [], [], [], []], comlen = []
        for (let i = 0; i < child0.length; i++) {
            len0[child0[i]['time'] - 1].push(i)
        }
        for (let i = 0; i < child1.length; i++) {
            len1[child1[i]['time'] - 1].push(i)
        }
        for (let i = 0; i < 6; i++) {
            if (len0[i].length && len1[i].length) {
                comlen.push(i + 1)
            }
        }
        //对理想后代多次交叉得到实际后代 这里设定为交换相同长度小段
        if (comlen.length) {
            let times = Math.floor(Math.random() * 3)
            for (let i = 0; i < times; i++) {
                let len = comlen[Math.floor(Math.random() * comlen.length)]
                let plen0 = len0[len - 1][Math.floor(Math.random() * len0[len - 1].length)]
                let plen1 = len1[len - 1][Math.floor(Math.random() * len1[len - 1].length)]
                let temp = child0[plen0]
                child0[plen0] = child1[plen1]
                child1[plen1] = temp
            }
        }
        //只保留两个后代中更优秀的那个
        tempeva0 = eva(child0)
        tempeva1 = eva(child1)
        if (tempeva1 <= tempeva0) {
            data.push(child0)
        }
        else {
            data.push(child1)
        }
    }
    //下面进行各种变异
    //简单移调
    var times = Math.floor(Math.random() * 2)
    for (let time = 0; time < times; time++) {
        let pd = Math.floor(Math.random() * data.length)
        let delta = Math.floor(Math.random() * 3) * (2 * Math.floor(Math.random() * 2) - 1)
        for (let xd = 0; xd < data[pd].length; xd++) {
            if (data[pd][xd]['pitch'].length) {
                let max = center, min = center
                for (let i = 0; i < data[pd][xd]['pitch'].length; i++) {
                    data[pd][xd]['pitch'][i] += delta
                    if (data[pd][xd]['pitch'][i] > max) max = data[pd][xd]['pitch'][i]
                    else if (data[pd][xd]['pitch'][i] < min) min = data[pd][xd]['pitch'][i]
                }
                let d = Math.floor(((max + min) / 2 - center) / 12)
                if (d != 0) {
                    for (let i = 0; i < data[pd][xd]['pitch'].length; i++) {
                        data[pd][xd]['pitch'][i] -= d * 12
                    }
                }
            }
        }
    }
    //倒影
    var times = Math.floor(Math.random() * 2)
    for (let time = 0; time < times; time++) {
        let pd = Math.floor(Math.random() * data.length)
        for (let xd = 0; xd < data[pd].length; xd++) {
            if (data[pd][xd]['pitch'].length) {
                for (let i = 0; i < data[pd][xd]['pitch'].length; i++) {
                    data[pd][xd]['pitch'][i] = 2 * center - data[pd][xd]['pitch'][i]
                }
            }
        }
    }
    //逆行
    var times = Math.floor(Math.random() * 2)
    for (let time = 0; time < times; time++) {
        let pd = Math.floor(Math.random() * data.length)
        data[pd].reverse()
    }
    //小变异
    var times = Math.floor(Math.random() * 2)
    for (let time = 0; time < times; time++) {
        let pd = Math.floor(Math.random() * data.length)
        let xd = Math.floor(Math.random() * data[pd].length)
        if (data[pd][xd]['pitch'].length) {
            let pp = Math.floor(Math.random() * data[pd][xd]['pitch'].length)
            let delta = Math.floor(Math.random() * 3) * (2 * Math.floor(Math.random() * 2) - 1)
            data[pd][xd]['pitch'][pp] += delta
            let d = Math.floor((data[pd][xd]['pitch'][pp] - center) / 12)
            if (d != 0) {
                data[pd][xd]['pitch'][pp] -= d * 12
            }
        }
    }
    //淘汰最差片段
    {
        let min = eva(data[0]), pmin = 0;
        for (let i = 0; i < data.length; i++) {
            let tempeva = eva(data[i])
            if (tempeva < min) {
                min = tempeva; pmin = i;
            }
        }
        data.splice(pmin, 1)
    }
}