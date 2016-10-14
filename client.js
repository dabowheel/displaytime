"use strict";
let $http = require("http-client-promise");

var body = {
  name: "momo"
};
var bodystr = JSON.stringify(body);
let options = {
  hostname: "localhost",
  port: 80,
  method: "POST",
  path: "/data/2.5/weather.cgi?q=albuquerque,us&units=imperial&appid=97deb1a62310b51f6a07885321377d5d",
  headers: {
    "Content-Type": "application/javascript",
    "Content-Length": bodystr.length
  }
};

var req = $http.request(options, bodystr).then(function (res) {
  return res.getData().then(function (body) {
    if (res.statusCode != 200) {
      console.log(body);
      return;
    }
    
    console.log(body);
  });
}).catch(function (err) {
  console.error(err);
});
