<template>
  <div id="toolbar" class="toolbar">
    <div class="toolbar-headers">
      <span v-if="$route.path == '/settings'" class="header">Settings</span>
      <span v-if="$route.path == '/signup'" class="header">Sign up</span>
      <span v-if="$route.path == '/login'" class="header">Login</span>
    </div>
    <div class="toolbar-links">
      <router-link v-if="$route.path != '/'" to="/" class="btn toolbar-link icon-link" title="close settings"><i class="fa fa-times fa-2x" aria-hidden="true"></i></router-link>
      <a href="#" v-on:click.prevent="clickMenu" class="btn toolbar-link menu-link" title="menu"><i class="fa fa-ellipsis-v fa-2x menu-icon" aria-hidden="true"></i></a>
    </div>
    <div class="toolbar-links">
      <router-link v-if="!$store.state.sessionID && $route.path == '/'" to="/signup" class="btn toolbar-link click-link" title="sign up">Sign up</router-link>
      <router-link v-if="!$store.state.sessionID && $route.path == '/'" to="/login" class="btn toolbar-link click-link" title="login">Login</router-link>
    </div>
    <div class="toolbar-separator"></div>
  </div>
</template>

<script>
  export default {
    methods: {
      clickMenu() {
        console.log("click menu");
      }
    }
  }
</script>

<style>
  /*
    mobile
  */
  .toolbar {
    margin-top: .5em;
    margin-left: 1em;
    margin-right: .5em;
  }
  .toolbar-headers {
    float: left;
  }
  .toolbar-links {
    float: right;
  }
  .toolbar-separator {
    clear: both;
    width: 100%;
    height: 1em;
  }
  .toolbar-link:link, .toolbar-link:visited, toolbar-link:hover, toolbar-link:active {
    text-decoration: none;
  }
  .click-link:hover {
  }
  .toolbar-link {
    color: purple;
  }
  .click-link {
    line-height: 2em;
    margin-left: .5em;
  }
  .icon-link {
    margin-left: .5em;
  }
  .menu-link {
  }
  .menu-icon {
    padding-left: .5em;
    padding-right: .5em;
  }
  .header {
    line-height: 1.3em;
    font-size: 1.5em;
  }
</style>
