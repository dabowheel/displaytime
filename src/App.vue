<template>
  <div id="app">
    <p>Hello</p>
    <p>The time is {{ time }}</p>
  </div>
</template>

<script>
import moment from 'moment';

export default {
  data () {
    return {
      time: moment().format("HH:MM:SS")
    }
  }
}
</script>

<style>
body {
  background-color: black;
  color: white;
  font-family: Helvetica, sans-serif;
}
</style>
