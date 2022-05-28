<template>
    <div class = "data-button">
        <button @click="test">{{ buttonLabel }}</button>
    </div>
</template>

<script>
export default {
    props: {
        buttonLabel: {
            type: String,
            default: 'Button'
        },
        topic: {
            type: String,
            default: ''
        }
    },

    methods: {
        test() {
            document.getElementById('chart-area').style.display = 'block';
        }
    }
}
</script>