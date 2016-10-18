<template>
  <div id="login">
    <div class="form">
      <div class="form-group">
        <label class='login-label'>Email Address:</label>
        <input type='text' class="login-field" v-model='email'>
      </div>
      <div class="form-group">
        <label class='login-label'>Password:</label>
        <input type='password' class="login-field" v-model='password'>
      </div>
      <div class="form-group">
        <button>Login</button>
      </div>
    </div>
  </div>
</template>

<script>
  export default {
    data () {
      return {
        email: "",
        password: "",
      }
    },
    computed: {
      
    }
  };
</script>

<style>
  #id {
    text-align: center;
    width: 100%;
  }
  .form {
    width: 250px;
    margin: 0 auto;
  }
  .form-group {
    margin-bottom: 1em;
  }
  .login-field {
    width: 100%;
  }
  input {
    box-sizing: border-box;
  }
</style>