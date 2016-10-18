<template>
  <div id="toolbar" class="toolbar">
    <div class="toolbar-headers">
      <span v-if="$route.path == '/settings'" class="header">Settings</span>
      <span v-if="$route.path == '/register'" class="header">Sign up</span>
      <span v-if="$route.path == '/login'" class="header">Login</span>
    </div>
    <div class="toolbar-links">
      <router-link v-if="$route.path == '/'" to="/register" class="btn toolbar-link click-link" title="sign up">Sign up</router-link>
      <router-link v-if="$route.path == '/'" to="/login" class="btn toolbar-link click-link" title="login">Login</router-link>
      <router-link v-if="$route.path == '/'" to="/settings" class="btn toolbar-link" title="show settings"><i class="fa fa-cog fa-2x click-icon settings-nav" aria-hidden="true"></i></router-link>
      <router-link v-if="$route.path != '/'" to="/" class="btn toolbar-link" title="close settings"><i class="fa fa-times fa-2x click-icon settings-nav" aria-hidden="true"></i></router-link>
    </div>
    <div class="toolbar-separator"></div>
  </div>
</template>

<style>
  /*
    mobile
  */
  .toolbar {
    margin-left: 1em;
    margin-right: 1em;
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
  .click-link:hover, .click-link:active {
    text-decoration: underline;
  }
  .toolbar-link:hover, .toolbar-link:active {
    color: black;
  }
  .toolbar-link {
      color: #444444;
  }
  .click-link {
    line-height: 2em;
    margin-right: .5em;
  }
  .click-icon {
    font-size: 2em;
  }
  .header {
    line-height: 2em;
    font-size: 1.5em;
  }
</style>
