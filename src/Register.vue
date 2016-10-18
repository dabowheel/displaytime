<template>
  <div id="register">
    <div class="form">
      <div class="form-group">
        <label class='register-label'>Email Address:</label>
        <input type='text' class="register-field" v-model='email'>
      </div>
      <div class="form-group">
        <label class='register-label'>Password:</label>
        <input type='password' class="register-field" v-model='password'>
      </div>
      <div class="form-group">
        <label class='register-label'>Confirm Password:</label>
        <input type='password' class="register-field" v-model='confirmPassword'>
      </div>
      <div class="form-group">
        <button>Sign Up</button>
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
        confirmPassword: ""
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
  .register-field {
    width: 100%;
  }
  input {
    box-sizing: border-box;
  }
</style>