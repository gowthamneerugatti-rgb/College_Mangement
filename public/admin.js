// Fetch all admins on page load
async function loadAdmins() {
    try {
        const response = await fetch('/api/admin/all');
        const data = await response.json();
        const tbody = document.querySelector('#adminTable tbody');
        tbody.innerHTML = '';
        data.forEach(admin => {
            tbody.innerHTML += `
                <tr>
                    <td>${admin.id}</td>
                    <td>${admin.name}</td>
                    <td>${admin.role}</td>
                    <td><button onclick="deleteAdmin(${admin.id})">Delete</button></td>
                </tr>
            `;
        });
    } catch (e) {
        console.error('Failed to load admins', e);
    }
}

// Add admin submission
document.getElementById('addAdminForm').addEventListener('submit', async (e) => {
    e.preventDefault();
    const newAdmin = {
        id: Number.parseInt(document.getElementById('adminId').value),
        name: document.getElementById('adminName').value,
        role: document.getElementById('adminRole').value
    };

    try {
        await fetch('/api/admin/add', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(newAdmin)
        });
        document.getElementById('addAdminForm').reset();
        loadAdmins();
    } catch (e) {
        console.error('Failed to add admin', e);
    }
});

async function deleteAdmin(id) {
    // Mocked delete function - normally requires DELETE API
    try {
        await fetch(`/api/admin/delete?id=${id}`, { method: 'DELETE' });
        loadAdmins();
    } catch (e) {
        console.error('Failed to delete admin', e);
    }
}

// Initial load
await loadAdmins();
