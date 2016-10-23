/*
  email,
  password,
*/
function packSignup(obj) {
  var ret = '';
  ret = 'email=' + encodeURIComponent(obj.email)
  ret += '&password=' + encodeURIComponent(obj.password)
  return ret
}

function  api (method, path, body, callback) {
  var request = new XMLHttpRequest()
  request.onreadystatechange = function () {
    if (request.readyState === XMLHttpRequest.DONE) {
      if (request.status !== 200) {
        console.error(request.status, request.responseText)
      }
      callback(request.status, request.responseText)
    }
  }
  request.open(method, 'http://' + location.host + '/api/' + path + '.cgi', true)
  if (body) {
    request.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded')
  }
  request.send(body)
}

export { packSignup, api};
