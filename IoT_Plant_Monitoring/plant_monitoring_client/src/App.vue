<template>
  <app-header />
  <app-main :items="items"/>
</template>

<script>
import AppHeader from './components/app-header.vue';
import AppMain from './components/app-main.vue';
import { ref } from 'vue';

export default {
    name: 'App',
    components: {
        AppHeader,
        AppMain
    },
    data() {
        const items = ref([
            {
                topic: '/iot/moisture',
                name: 'soil moisture',
                properties: {
                    min: 0,
                    max: 100
                }
            },
            {
                topic: '/iot/water', 
                name: 'water level',
                properties: {
                    min: 0,
                    max: 100
                }
            },
            {
                topic: '/iot/light', 
                name: 'light level',
                properties: {

                }
            }, 
            {
                topic: '/iot/temperature', 
                name: 'air temperature',
                properties: {

                }
            },
            {
                topic: '/iot/humidity', 
                name: 'air humidity',
                properties: {
                    min: 0,
                    max:100
                }
            }
        ]);

        return {
            items
        };
    }
}
</script>

<style>
body {
    background-color: #FDFFE7;
    color: #393A3C;

    font-family: Avenir, Helvetica, Arial, sans-serif;
    -webkit-font-smoothing: antialiased;
    -moz-osx-font-smoothing: grayscale;
    text-align: center;
    
    margin-top: 60px;
}

</style>
