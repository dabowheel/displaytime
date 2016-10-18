import Vue from 'vue'
import VueRouter from 'vue-router'
import Vuex from 'vuex'
import moment from 'moment'
import App from './App.vue'
import Settings from './Settings.vue'

Vue.use(VueRouter)
Vue.use(Vuex)

const routes = [
  {
    path: '/settings',
    component: Settings
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
    datetimeString: datetimeString
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
