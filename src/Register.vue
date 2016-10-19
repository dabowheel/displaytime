<template>
  <div id="register">
    <div class="form">
      <div class="form-group">
        <label class='register-label'>Email Address:</label>
        <input id='email' type='text' class="register-field" v-model='email'>
      </div>
      <div class="form-group">
        <label class='register-label'>Password:</label>
        <input id='password' type='password' class="register-field" v-model='password'>
      </div>
      <div class="form-group">
        <label class='register-label'>Confirm Password:</label>
        <input id='confirmPassword' type='password' class="register-field" v-model='confirmPassword'>
      </div>
      <div class="form-group">
        <button v-bind:disabled="valid == false">Sign Up</button>
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
      checkEmail() {
        if (!this.email)
          return "Email needs a value"
        if (this.email.indexOf('@') == -1)
          return "Email must be a valid email address"
        return ""
      },
      checkPassword() {
        if (!this.password)
          return "Password needs a value"
        if (this.password.length < 8)
          return "Password must be at least 8 characters"
        return ""
      },
      checkConfirmPassword() {
        if (this.checkPassword)
          return ""
        if (!this.confirmPassword)
          return "Confirm Password needs a value"
        if (this.confirmPassword != this.password)
          return "Confirm Password must match Password"
        return ""
      },
      valid() {
        return (this.checkEmail + this.checkPassword + this.checkConfirmPassword) == ""
      }
    },
    watch: {
      email() {
        document.getElementById("email").setCustomValidity(this.checkEmail)
      },
      password() {
        document.getElementById("password").setCustomValidity(this.checkPassword)
        document.getElementById("confirmPassword").setCustomValidity(this.checkConfirmPassword)
      },
      confirmPassword() {
        document.getElementById("confirmPassword").setCustomValidity(this.checkConfirmPassword)
      }
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