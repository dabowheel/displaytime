<template>
  <div id="signup">
    <form id="signUpForm" class="form">
      <div class="form-group">
        <label for='email' class='signup-label'>Email Address:</label>
        <input id='email' type='email' v-bind:class='{fieldError: validateEmail && emailError}' class="signup-field" v-model='email'>
        <div v-if='validateEmail && emailError != ""' class='error'>
          {{ emailError}}
        </div>
      </div>
      <div class="form-group">
        <label for='password' class='signup-label'>Password:</label>
        <input id='password' type='password' v-bind:class='{fieldError: validatePassword && passwordError}' class="signup-field" v-model='password'>
        <div v-if='validatePassword && passwordError != ""' class='error'>
          {{ passwordError }}
        </div>
      </div>
      <div class="form-group">
        <label for='confirmPassword' class='signup-label'>Confirm Password:</label>
        <input id='confirmPassword' type='password' v-bind:class='{fieldError: validateConfirmPassword && confirmPasswordError}' class="signup-field" v-model='confirmPassword'>
        <div v-if='validateConfirmPassword && confirmPasswordError != ""' class="error">
          {{ confirmPasswordError }}
        </div>
      </div>
      <div class="form-group">
        <button v-on:click.prevent="clickSignUp" type="submit" v-bind:disabled="!valid">Sign Up</button>
      </div>
    </form>
  </div>
</template>

<script>
  import {packSignup, api, decodeForm} from './com'

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

        if (this.valid) {
          var body = packSignup({
            email: this.email,
            password: this.password
          })

          api('POST', 'signup', body, function (status, body) {
            if (status !== 200) {
              this.$store.commit('setLastError', "There was an error during signup. Please contact the administrator or try again later.")
              this.$router.push('error')
              return;
            }

            var obj = decodeForm(body);
            if (!obj.sessionID) {
              console.error("sessionID not returned on signup");
              this.$store.commit('setLastError', "There was an error during signup. Please contact the administrator or try again later.")
              this.$router.push('error')
              return;
            }

            sessionStorage.sessionID = obj.sessionID;
            this.$store.commit('setSessionID', obj.sessionID);
            this.$router.push('/')
          }.bind(this))
        }
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
  .signup-field {
    width: 100%;
  }
  input {
    box-sizing: border-box;
  }
  .fieldError {
    border-color: red;
  }
  .error {
    color: red;
  }
</style>