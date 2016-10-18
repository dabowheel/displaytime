<template>
  <div id="app">
    <toolbar></toolbar>
    <router-view></router-view>
    <div v-if="$route.path == '/'" id="datetime">{{ datetimeString }}</div>
  </div>
</template>

<script>
import moment from 'moment';
import Toolbar from './Toolbar.vue';

export default {
  computed: {
    datetimeString() {
      return this.$store.state.datetimeString;
    }
  },
  components: {
    toolbar: Toolbar
  }
}
</script>

<style>
/*
  mobile
*/
body {
  font-family: Helvetica, sans-serif;
  margin: 0;
  padding: 0;
}
#datetime {
  text-align: center;
  font-size: 2em;
}
</style>
