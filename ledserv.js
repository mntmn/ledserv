const express = require('express')
const serialport = require('serialport')
const app = express()
const glob = require("glob")
const os = require('os')
const ifaces = os.networkInterfaces()

const baud = 57600

Object.keys(ifaces).forEach(function (ifname) {
  ifaces[ifname].forEach(function (iface) {
    console.log(ifname, iface.address)
  })
})

glob("/dev/ttyUSB*", function (er, devices) {
  const device = devices[0]
  console.log("device:",device)
  
  const arduino = new serialport(device, {
    baudRate: baud
  })

  arduino.on('data', function (data) {
    console.log('arduino:', data+"");
  })

  app.get('/control', function (req, res) {
    var cmd = req.query.cmd
    arduino.write(cmd+"\r", function(err) {
      if (err) {
        res.send('error: '+err)
      } else {
        res.send('ok: '+cmd)
      }
    })
  })

  app.post('/update-fw', function (req, res) {
    process.execSync('avrdude -patmega328p -carduino -P'+device+' -b'+baud+' -D -Uflash:w:ledctrl.hex:i')
    res.send('ok')
  })

  app.listen(3000, function () {
    console.log('ledserv started.')
  })
})
