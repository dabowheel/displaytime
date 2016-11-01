<template>
  <div>
    <form class='form'>
      <div class='form-group'>
        <label for='email'>Email Address:</label>
        <input v-model='email' class='form-control' type='email' name='email'>
        <div v-show='validateEmail' class="error">
          {{ emailError }}
        </div>
      </div>
      <div class='form-group'>
        <label for='password'>Old Password:</label>
        <input v-model='password' class='form-control' type='password' name='password'>
        <div v-show='validatePassword' class="error">
          {{ passwordError }}
        </div>
      </div>
      <div class='form-group'>
        <label for='new-password'>New Password:</label>
        <input v-model='newPassword' class='form-control' type='password' name='new-password'>
        <div v-show='validateNewPassword' class="error">
          {{ newPasswordError }}
        </div>
      </div>
      <button v-bind:click='clickSave' type='submit' v-bind:disabled='!valid'>Save</button>
    </form>
  </div>
</template>

<script>
  import {api, decodeForm, packSignup} from './com'
  export default {
    data() {
      return {
        email: '',
        password: '',
        newPassword: '',
        validateEmail: false,
        validatePassword: false,
        validateNewPassword: false
      }
    },
    created() {
      var query_string = "sessionID=" + encodeURIComponent(this.$store.state.sessionID)
      api("GET", 'profile', query_string, null, function (status, body) {
        if (status != 200) {
          this.$store.commit('setLastError', 'The was an error getting profile information.')
          this.$router.push('error')
          return
        }

        var obj = decodeForm(body);
        if (!obj.email) {
          this.$store.commit('setLastError', 'The was an error getting profile information.')
          this.$router.push('error') 
          console.error('email not found in response')
          return         
        }

        this.email = obj.email
      }.bind(this));
    },
    computed: {
      emailError() {
        if (!this.email)
          return "Email needs a value"
        if (!((/[^ @]+@[^ @]+/).exec(this.email)))
          return "Email must be a valid email address"
        return ""
      },
      passwordError () {
        if (this.newPassword && !this.password)
          return 'Old Password must be entered to change passwords'
        return ""
      },
      newPasswordError() {
        if (this.newPassword && this.newPassword.length < 8)
          return "New Password must be at least 8 characters"
        return ""
      },
      valid() {
        return (this.emailError + this.passwordError + this.newPasswordError) == ""
      }
    },
    methods: {
      clickSave() {
        this.validateEmail = true
        this.validatePassword = true
        this.validateNewPassword = true
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
      newPassword() {
        if (this.newPassword)
          this.validateNewPassword = true
      }
    }
  }
</script>

<style scoped>
  /* mobile */
  form {
    width: 250px;
  }
  .form-control {
    width: 100%;
  }

  /* desktop */
  @media (min-width: 1000px) {
    .form-control {
      width: 200px;
    }
  }
</style>
