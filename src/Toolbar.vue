<template>
  <div id="toolbar" class="toolbar">
    <div id="toolbar-links" class="toolbar-links">
    <a v-if="$route.path == '/'" href="#" class="btn toolbar-link click-link" title="sign up">Sign up</a>
    <a v-if="$route.path == '/'" href="#" class="btn toolbar-link click-link" title="login">Login</a>
    <span v-if="$route.path == '/settings'" class="header">Settings</span>
    <router-link v-if="$route.path == '/'" to="/settings" class="btn toolbar-link" title="show settings"><i class="fa fa-cog fa-2x click-icon settings-nav" aria-hidden="true"></i></router-link>
    <router-link v-if="$route.path == '/settings'" to="/" class="btn toolbar-link" title="close settings"><i class="fa fa-times fa-2x click-icon settings-nav" aria-hidden="true"></i></router-link>
    </div>
  </div>
</template>

<style>
  /*
    mobile
  */
  .toolbar-links {
    text-align: right;
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

  .toolbar {
    margin-right: 1em;
    margin-bottom: 1em;
  }
  .click-link {
    font-height: 2em;
    margin-right: .5em;
  }
  .click-icon {
    font-size: 2em;
  }
  .header {
    font-size: 1.5em;
  }
</style>
