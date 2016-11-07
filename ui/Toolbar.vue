<template>
  <div>
    <div class="drop-menu drop-menu-left">
      <div v-if='$route.path == "/settings"' class="drop-menu-header">
        <span class='header'>Settings</span>
      </div>
    </div>
    <div v-bind:class="{ 'drop-menu-expanded': menuExpanded }" class="drop-menu">
      <div v-show="!menuExpanded" class="drop-menu-header">
        <a href="#" v-on:click.prevent="clickMenu" class="" title="menu"><i class="fa fa-ellipsis-v fa-2x drop-menu-icon" aria-hidden="true"></i></a>
      </div>
      <div v-show="menuExpanded" class="drop-menu-header">
        <a href="#" v-on:click.prevent="clickMenu" class="" title="close"><i class="fa fa-times fa-2x drop-menu-icon" aria-hidden="true"></i></a>
      </div>
      <div v-if="menuExpanded && $route.path != '/'" class="drop-menu-item">
        <a v-on:click.prevent="clickHome" href="/" class="" title="home">Home</a>
      </div>
      <div v-show="menuExpanded && !sessionID && $route.path != '/signup'" class="drop-menu-item">
        <a v-on:click.prevent="clickSignup" href="/signup" class="" title="sign up">Sign up</a>
      </div>
      <div v-show="menuExpanded && !sessionID && $route.path != '/login'" class="drop-menu-item">
        <a v-on:click.prevent="clickLogin" href="/login" class="" title="login">Login</a>
      </div>
      <div v-show="menuExpanded && $route.path != '/settings'" class="drop-menu-item">
        <a v-on:click.prevent="clickSettings" href="/settings" class="" title="settings">Settings</a>
      </div>
      <div v-show="menuExpanded && sessionID && $route.path != '/profile'" class="drop-menu-item">
        <a v-on:click.prevent="clickProfile" href="/profile" class="" title="profile">Profile</a>
      </div>
      <div v-show="menuExpanded && sessionID && $route.path != '/logout'" class="drop-menu-item">
        <a v-on:click.prevent="clickLogout" href="/logout" class="" title="logout">Logout</a>
      </div>
    </div>

    <div class="seperator"></div>
  </div>
</template>

<script>
  export default {
    data() {
      return {
        menuExpanded: false
      }
    },
    computed: {
      sessionID() {
        return this.$store.state.sessionID
      }
    },
    methods: {
      clickMenu() {
        this.menuExpanded = !this.menuExpanded
      },
      clickHome() {
        this.menuExpanded = !this.menuExpanded
        this.$router.push('/')
      },
      clickSignup() {
        this.menuExpanded = !this.menuExpanded
        this.$router.push('signup')
      },
      clickLogin() {
        this.menuExpanded = !this.menuExpanded
        this.$router.push('login')
      },
      clickSettings() {
        this.menuExpanded = !this.menuExpanded
        this.$router.push('settings')
      },
      clickProfile() {
        this.menuExpanded = !this.menuExpanded
        this.$router.push('profile')
      },
      clickLogout() {
        this.$store.commit('setSessionID', '')
        this.$store.commit('setSessionExpire', '')
        this.menuExpanded = !this.menuExpanded
        this.$router.push('/')
      }
    }
  }
</script>

<style scoped>
  .drop-menu {
    position: absolute;
    right: 0px;
    width: 100px;
    padding-top: 1em;
    padding-left: 1em;
    padding-right: 1em;
  }
  .drop-menu-expanded {
    background-color: lightgray;
  }
  .drop-menu-header {
    text-align: right;
    padding-bottom: 1em;
  }
  .drop-menu-icon {
    width: 1em;
    text-align: center;
  }
  .drop-menu-item {
    text-align: center;
    padding-bottom: 1em;
  }
  .seperator {
    height: 4em;
    width: 100%;
  }
  .drop-menu-left {
    right: auto;
  }
  .header {
    font-size: 1.73906em;
  }
</style>
