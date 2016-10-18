<template>
  <div id="register">
    <div class="form-group">
      <label>Email Address:</label>
      <input type='text' class="register-field" v-model='email'/>
      <label>Password:</label>
      <input type='password' class="register-field" v-model='password'/>
    </div>
    <div class="form-group">
      <button>Sign Up</button>
    </div>
  </div>
</template>

<script>
  export default {
    data () {
      return {
        email: "",
        password: ""
      }
    }
  };
</script>

<style>
  #id {
    text-align: center;
    width: 100%;
  }
  .form-group {
    width: 250px;
    margin: 0 auto;
  }
  .register-field {
    width: 100%;
  }
</style>