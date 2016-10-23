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
  }
];
const router = new VueRouter({
  mode: 'history',
  routes: routes
});

const datetime = moment()
const datetimeFormat = localStorage.datetimeFormat ? localStorage.datetimeFormat : 'lll'
const datetimeString = datetime.format(datetimeFormat)

const store = new Vuex.Store({
  state: {
    datetime: datetime,
    datetimeFormat: datetimeFormat,
    datetimeString: datetimeString,
    lastError: ''
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
      state.lastError = error;
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
