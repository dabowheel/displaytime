<template>
  <div>
    <div v-bind:class="{ 'drop-menu-expanded': menuExpanded }" class="drop-menu">
      <div v-show="!menuExpanded" class="drop-menu-header">
        <a href="#" v-on:click.prevent="clickMenu" class="" title="menu"><i class="fa fa-ellipsis-v fa-2x drop-menu-icon" aria-hidden="true"></i></a>
      </div>
      <div v-show="menuExpanded" class="drop-menu-header">
        <a href="#" v-on:click.prevent="clickMenu" class="" title="close"><i class="fa fa-times fa-2x drop-menu-icon" aria-hidden="true"></i></a>
      </div>
      <div v-show="menuExpanded" class="drop-menu-item">
        <a v-on:click.prevent="clickHome" href="/" class="" title="home">Home</a>
      </div>
      <div v-show="menuExpanded" class="drop-menu-item">
        <a v-on:click.prevent="clickSignup" href="/signup" class="" title="sign up">Sign up</a>
      </div>
      <div v-show="menuExpanded" class="drop-menu-item">
        <a v-on:click.prevent="clickLogin" href="/login" class="" title="login">Login</a>
      </div>
      <div v-show="menuExpanded" class="drop-menu-item">
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
      clickLogout() {
        this.menuExpanded = !this.menuExpanded
        this.$router.push('logout')
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
</style>
