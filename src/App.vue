<template>
  <div id="app">
    <p id="greeting">Hello</p>
    <p id="time">{{ time }}</p>
  </div>
</template>

<script>
import moment from 'moment';

export default {
  data () {
    return {
      time: moment().format("hh:mm:ss")
    }
  },
  created() {
    setInterval(function () {
      this.time = moment().format("hh:mm:ss");
    }.bind(this), 500);
  }
}
</script>

<style>
body {
  background-color: black;
  color: white;
  font-family: Helvetica, sans-serif;
}
p {
  text-align: center;
}
#greeting {
  font-size: 20pt;
}
#time {
  font-size: 60pt;
}
</style>
