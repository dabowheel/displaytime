<template>
  <div id="login">
    <div class="form">
      <div class="form-group">
        <label class='login-label'>Email Address:</label>
        <input type='email' v-bind:class='{ fieldError: validateEmail && emailError }' class="login-field" v-model='email'>
        <div v-if='validateEmail && emailError' class='error'>
          {{ emailError }}
        </div>
      </div>
      <div class="form-group">
        <label class='login-label'>Password:</label>
        <input type='password' v-bind:class='{ fieldError: validatePassword && passwordError }' class="login-field" v-model='password'>
        <div v-if='validatePassword && passwordError' class='error'>
          {{ passwordError}}
        </div>
      </div>
      <div class="form-group">
        <button v-on:click.prevent='clickLogin' v-bind:disabled='!valid'>Login</button>
      </div>
    </div>
  </div>
</template>

<script>
  import {api, decodeForm, packSignup, packLogin} from './com'
  export default {
    data () {
      return {
        email: "",
        password: "",
        validateEmail: false,
        validatePassword: false
      }
    },
    computed: {
      emailError() {
        if (!this.email)
          return "Email Address needs a value"
        if (!(/[^ @]+@[^ @]+/).exec(this.email))
          return "Invalid email address"
        return ""
      },
      passwordError() {
        if (!this.password)
          return "Password needs a value"
        return ""
      },
      valid() {
        return (this.emailError + this.passwordError) == ""
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
      }
    },
    methods: {
      clickLogin() {
        console.log('click login')
        this.validateEmail = true
        this.validatePassword = true
        if (this.valid) {
          var body = packLogin(this.email, this.password)
          api('POST', 'login', body, function (status, body) {
            if (status != 200) {
              this.$store.commit('setLastError', "There was an error during signup. Please contact the administrator or try again later.")
              this.$router.push('error')
              return
            }

            console.log(body)
            var obj = decodeForm(body)
            if (!obj.sessionID || !obj.sessionExpire) {
              console.error("session ID or expire not returned on signup");
              this.$store.commit('setLastError', 'There was an error during login. Please contact the administrator or try again later.')
              this.$router.push('error')
              return
            }

            this.$store.commit('setSessionID', obj.sessionID);
            this.$store.commit('setSessionExpire', obj.sessionExpire);
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
  .login-field {
    width: 100%;
  }
  input {
    box-sizing: border-box;
  }
  .field-error {
    border-color: red;
  }
  .error {
    color: red;
  }
</style>