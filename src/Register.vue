<template>
  <div id="register">
    <form id="signUpForm" class="form">
      <div class="form-group">
        <label for='email' class='register-label'>Email Address:</label>
        <input id='email' type='text' class="register-field" data-h5-errorid="invalid-email" required title='Email is required' v-model='email'>
        <div v-if='validateEmail && emailError != ""' class='error'>
          {{ emailError}}
        </div>
      </div>
      <div class="form-group">
        <label for='password' class='register-label'>Password:</label>
        <input id='password' type='password' class="register-field" data-h5-errorid='invalid-password' required title='Password is required' v-model='password'>
        <div v-if='validatePassword && passwordError != ""' class='error'>
          {{ passwordError }}
        </div>
      </div>
      <div class="form-group">
        <label for='confirmPassword' class='register-label'>Confirm Password:</label>
        <input id='confirmPassword' type='password' class="register-field" data-h5-errorid='invalid-confirm-password' required title='Confirm Password is required' v-model='confirmPassword'>
        <div v-if='validateConfirmPassword && confirmPasswordError != ""' class="error">
          {{ confirmPasswordError }}
        </div>
      </div>
      <div class="form-group">
        <button v-on:click.prevent="clickSignUp" type="submit">Sign Up</button>
      </div>
    </form>
  </div>
</template>

<script>
  export default {
    data () {
      return {
        email: "",
        password: "",
        confirmPassword: "",
        validateEmail: false,
        validatePassword: false,
        validateConfirmPassword: false
      }
    },
    computed: {
      emailError() {
        if (!this.email)
          return "Email needs a value"
        if (!((/[^ @]+@[^ @]+/).exec(this.email)))
          return "Email must be a valid email address"
        return ""
      },
      passwordError() {
        if (!this.password)
          return "Password needs a value"
        if (this.password.length < 8)
          return "Password must be at least 8 characters"
        return ""
      },
      confirmPasswordError() {
        if (this.checkPassword)
          return ""
        if (!this.confirmPassword)
          return "Confirm Password needs a value"
        if (this.confirmPassword != this.password)
          return "Confirm Password must match Password"
        return ""
      },
      valid() {
        return true;
        return (this.emailError + this.passwordError + this.confirmPasswordError) == ""
      }
    },
    watch: {
      email() {
        if (this.email)
          this.validateEmail = true
      },
      password() {
        if (this.password)
          this.validatePassword = true
      },
      confirmPassword() {
        if (this.confirmPassword)
          this.validateConfirmPassword = true
      }
    },
    methods: {
      clickSignUp() {
        this.validateEmail = true
        this.validatePassword = true
        this.validateConfirmPassword = true
      }
    }
  }
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
  .error {
    color: red;
  }
</style>