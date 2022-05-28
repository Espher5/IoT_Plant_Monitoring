<template> 
    <div class="data-buttons">
        <menu-item v-for="item in items" :key="item.topic" :buttonLabel="item.name"></menu-item>
    </div> 
    <div id="chart-area">
        <sensor-graph  v-for="item in items" :key="item.topic" :topic="item.topic" :name="item.name"/>
    </div>  
</template>

<script>
import menuItem from './menu-item.vue'
import SensorGraph from './sensor-graph.vue';

export default {
    components: { menuItem, SensorGraph },
    props: {
        items: {
            type: Array,
            default: null
        }
    },


}
</script>
