import Vue from 'vue'
import VueRouter from 'vue-router'
import Vuex from 'vuex'
import moment from 'moment'
import App from './App.vue'
import Settings from './Settings.vue'
import Signup from './Signup.vue'
import Login from './Login.vue'
import Error from './Error.vue'
import SignupResult from './SignupResult.vue'
import Profile from './Profile.vue'

Vue.use(VueRouter)
Vue.use(Vuex)

const routes = [
  {
    path: '/settings',
    component: Settings
  },
  {
    path: '/signup',
    component: Signup
  },
  {
    path: '/login',
    component: Login
  },
  {
    path: '/error',
    component: Error
  },
  {
    path: '/signupResult',
    component: SignupResult
  },
  {
    path: '/profile',
    component: Profile
  }
];
const router = new VueRouter({
  mode: 'history',
  routes: routes
});

const datetime = moment()
const datetimeFormat = localStorage.datetimeFormat ? localStorage.datetimeFormat : 'lll'
const datetimeString = datetime.format(datetimeFormat)
const sessionID = sessionStorage.sessionID
const sessionExpire = sessionStorage.sessionExpire

const store = new Vuex.Store({
  state: {
    datetime: datetime,
    datetimeFormat: datetimeFormat,
    datetimeString: datetimeString,
    lastError: '',
    sessionID: sessionID,
    sessionExpire: sessionExpire
  },
  mutations: {
    updateDatetime(state) {
      state.datetime = moment()
      state.datetimeString = state.datetime.format(state.datetimeFormat)
    },
    setDatetimeFormat(state, newFormat) {
      state.datetimeFormat = newFormat
      state.datetimeString = state.datetime.format(newFormat)
      localStorage.datetimeFormat = newFormat
    },
    setLastError(state, error) {
      state.lastError = error
    },
    setSessionID(state, id) {
      state.sessionID = id;
      sessionStorage.sessionID = id
    },
    setSessionExpire(state, expire) {
      state.sessionExpire = expire
      sessionStorage.sessionExpire = expire
    }
  }
})

setInterval(function () {
  store.commit('updateDatetime')
}, 1000)

new Vue({
  el: '#app',
  router,
  store,
  render: h => h(App)
})
