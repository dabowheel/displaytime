<template>
  <div class="toolbar">
    <div class="toolbar-links">
    <a v-show='showSignup' @click.prevent="clickSignup" href="#" class="btn toolbar-link click-link" title="sign up">Sign up</a>
    <a v-show='showLogin' @click.prevent="clickLogin" href="#" class="btn toolbar-link click-link" title="login">Login</a>
    <span class="header">Settings</span>
    <a v-show='showOpenSettings' @click.prevent="clickOpenSettings" href="#" class="btn toolbar-link" title="show settings"><i class="fa fa-cog fa-2x click-icon settings-nav" aria-hidden="true"></i></a>
    <a v-show='showCloseSettings' @click.prevent="clickCloseSettings" href="#" class="btn toolbar-link" title="show settings"><i class="fa fa-times fa-2x click-icon settings-nav" aria-hidden="true"></i></a>
    </div>
  </div>
</template>

<script>
  export default {
    props: [
      'showSignup',
      'showLogin',
      'showOpenSettings',
      'showCloseSettings'
    ],
    methods: {
      clickSignup() {
        this.$emit('clickSignup');
      },
      clickLogin() {
        this.$emit('clickLogin');
      },
      clickSettings() {
        this.$emit('clickSettings');
      }
      clickCloseSettings() {
        this.$emit('clickCloseSettings');
      }
    }
  }
</script>

<style>
  /*
    Mobile
  */
  .toolbar-links {
    text-align: right;
  }
  .toolbar-link:link, .toolbar-link:visited {
    text-decoration: none;
  }
  .toolbar-link:hover, .toolbar-link:active {
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
  }
  .click-link {
    font-height: 2em;
    margin-right: .5em;
  }
  .header {
    font-size: 1.5em;
  }
  .toolbar {
    margin-bottom: 1em;
  }

  /*
    Large Screen
  */
  @media (min-width: 1000px) {
    /* Font Size */
    .click-icon {
      font-size: 2em;
    }
  }
</style>
